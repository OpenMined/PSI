
#include "psi_cardinality_client.h"
#include <vector>
#include "absl/memory/memory.h"
#include "absl/strings/str_cat.h"
#include "openssl/obj_mac.h"
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "util/canonical_errors.h"
#include "util/status_macros.h"

namespace psi_cardinality {

PSICardinalityClient::PSICardinalityClient(
    std::unique_ptr<::private_join_and_compute::ECCommutativeCipher> ec_cipher)
    : ec_cipher_(std::move(ec_cipher)) {}

StatusOr<std::unique_ptr<PSICardinalityClient>> PSICardinalityClient::Create() {
  // Create an EC cipher with curve secp224r1. This gives 112 bits of security.
  ASSIGN_OR_RETURN(
      auto ec_cipher,
      ::private_join_and_compute::ECCommutativeCipher::CreateWithNewKey(
          NID_secp224r1,
          ::private_join_and_compute::ECCommutativeCipher::HashType::SHA256));
  return absl::WrapUnique(new PSICardinalityClient(std::move(ec_cipher)));
}

StatusOr<std::string> PSICardinalityClient::CreateRequest(
    absl::Span<const std::string> inputs) {
  // Encrypt inputs one by one and store in a JSON array.
  rapidjson::Document request;
  request.SetArray();
  for (int64_t i = 0; i < static_cast<int64_t>(inputs.size()); i++) {
    ASSIGN_OR_RETURN(std::string encrypted_input,
                     ec_cipher_->Encrypt(inputs[i]));
    request.PushBack(rapidjson::Value().SetString(encrypted_input.data(),
                                                  encrypted_input.size(),
                                                  request.GetAllocator()),
                     request.GetAllocator());
  }

  // Return encrytped inputs as JSON array.
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  request.Accept(writer);
  return std::string(buffer.GetString());
}

StatusOr<int64_t> PSICardinalityClient::ProcessResponse(
    const std::string& server_setup, const std::string& server_response) {
  // Parse setup and response message as JSON.
  rapidjson::Document setup, response;
  if (setup.Parse(server_setup.c_str()).HasParseError()) {
    return ::private_join_and_compute::InvalidArgumentError(
        absl::StrCat("Error parsing `server_setup`: ",
                     rapidjson::GetParseError_En(setup.GetParseError()), "(",
                     setup.GetErrorOffset(), ")"));
  }
  if (response.Parse(server_response.c_str()).HasParseError()) {
    return ::private_join_and_compute::InvalidArgumentError(
        absl::StrCat("Error parsing `server_response`: ",
                     rapidjson::GetParseError_En(response.GetParseError()), "(",
                     response.GetErrorOffset(), ")"));
  }

  // TODO: Decode Bloom filter.

  // Decrypt all elements in the response.
  int64_t counter = 0;
  if (!response.IsArray()) {
    return ::private_join_and_compute::InvalidArgumentError(
        "`server_response` must be a JSON array");
  }
  for (const auto& value : response.GetArray()) {
    if (!value.IsString()) {
      return ::private_join_and_compute::InvalidArgumentError(
          "`server_response` elements must be strings");
    }
    ASSIGN_OR_RETURN(std::string element,
                     ec_cipher_->Decrypt(std::string(value.GetString(),
                                                     value.GetStringLength())));
    // TODO: Check if element is in the bloom filter and increase counter if it is.
  }
  return counter;
}

}  // namespace psi_cardinality