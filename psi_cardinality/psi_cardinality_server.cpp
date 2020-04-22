//
// Copyright 2020 the authors listed in CONTRIBUTORS.md
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "psi_cardinality_server.h"
#include <vector>
#include "absl/memory/memory.h"
#include "absl/strings/str_cat.h"
#include "openssl/obj_mac.h"
#include "psi_cardinality/bloom_filter.h"
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "util/canonical_errors.h"
#include "util/status_macros.h"

namespace psi_cardinality {

PSICardinalityServer::PSICardinalityServer(
    std::unique_ptr<::private_join_and_compute::ECCommutativeCipher> ec_cipher)
    : ec_cipher_(std::move(ec_cipher)) {}

StatusOr<std::unique_ptr<PSICardinalityServer>>
PSICardinalityServer::CreateWithNewkey() {
  // Create an EC cipher with curve P-256. This gives 128 bits of security.
  ASSIGN_OR_RETURN(
      auto ec_cipher,
      ::private_join_and_compute::ECCommutativeCipher::CreateWithNewKey(
          NID_X9_62_prime256v1,
          ::private_join_and_compute::ECCommutativeCipher::HashType::SHA256));
  return absl::WrapUnique(new PSICardinalityServer(std::move(ec_cipher)));
}

StatusOr<std::unique_ptr<PSICardinalityServer>>
PSICardinalityServer::CreateFromKey(const std::string& key_bytes) {
  // Create an EC cipher with curve P-256. This gives 128 bits of security.
  ASSIGN_OR_RETURN(
      auto ec_cipher,
      ::private_join_and_compute::ECCommutativeCipher::CreateFromKey(
          NID_X9_62_prime256v1, key_bytes,
          ::private_join_and_compute::ECCommutativeCipher::HashType::SHA256));
  return absl::WrapUnique(new PSICardinalityServer(std::move(ec_cipher)));
}

StatusOr<std::string> PSICardinalityServer::CreateSetupMessage(
    double fpr, absl::Span<const std::string> inputs) const {
  // Create a Bloom filter and insert elements into it.
  int64_t num_inputs = static_cast<int64_t>(inputs.size());
  ASSIGN_OR_RETURN(auto bloom_filter, BloomFilter::Create(fpr, num_inputs));
  for (int i = 0; i < num_inputs; i++) {
    ASSIGN_OR_RETURN(std::string encrypted_element,
                     ec_cipher_->Encrypt(inputs[i]));
    bloom_filter->Add(encrypted_element);
  }

  // Encode Bloom filter as JSON and return it.
  rapidjson::Document setup;
  setup.SetObject();
  setup.AddMember("num_hash_functions",
                  rapidjson::Value().SetInt(bloom_filter->NumHashFunctions()),
                  setup.GetAllocator());
  setup.AddMember("bits",
                  rapidjson::Value().SetString(bloom_filter->ToString().data(),
                                               bloom_filter->ToString().size()),
                  setup.GetAllocator());
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  setup.Accept(writer);
  return std::string(buffer.GetString());
}

StatusOr<std::string> PSICardinalityServer::ProcessRequest(
    const std::string& client_request) const {
  // Unpack client message.
  rapidjson::Document request;
  request.Parse(client_request.data(), client_request.size());
  if (request.HasParseError()) {
    return ::private_join_and_compute::InvalidArgumentError(
        absl::StrCat("Error parsing `client_request`: ",
                     rapidjson::GetParseError_En(request.GetParseError()), "(",
                     request.GetErrorOffset(), ")"));
  }
  if (!request.IsArray()) {
    return ::private_join_and_compute::InvalidArgumentError(
        "`client_request` must be a JSON array");
  }

  // Re-encrypt elements and sort the resulting ciphertexts.
  int64_t num_client_elements = static_cast<int64_t>(request.GetArray().Size());
  std::vector<std::string> reencrypted_elements(num_client_elements);
  for (int i = 0; i < num_client_elements; i++) {
    if (!request[i].IsString()) {
      return ::private_join_and_compute::InvalidArgumentError(
          "`client_request` elements must be strings");
    }
    ASSIGN_OR_RETURN(
        reencrypted_elements[i],
        ec_cipher_->ReEncrypt(
            std::string(request[i].GetString(), request[i].GetStringLength())));
  }
  std::sort(reencrypted_elements.begin(), reencrypted_elements.end());

  // Encode re-encrypted elements as JSON.
  rapidjson::Document response;
  response.SetArray();
  for (int i = 0; i < num_client_elements; i++) {
    response.PushBack(
        rapidjson::Value().SetString(reencrypted_elements[i].data(),
                                     reencrypted_elements[i].size()),
        response.GetAllocator());
  }
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  response.Accept(writer);
  return std::string(buffer.GetString());
}

std::string PSICardinalityServer::GetPrivateKeyBytes() const {
  return ec_cipher_->GetPrivateKeyBytes();
}

}  // namespace psi_cardinality