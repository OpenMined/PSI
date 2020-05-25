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

#include "private_set_intersection/cpp/psi_client.h"

#include <vector>

#include "absl/memory/memory.h"
#include "absl/strings/escaping.h"
#include "absl/strings/str_cat.h"
#include "openssl/obj_mac.h"
#include "private_set_intersection/cpp/bloom_filter.h"
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "util/canonical_errors.h"
#include "util/status_macros.h"

namespace private_set_intersection {

PsiClient::PsiClient(
    std::unique_ptr<::private_join_and_compute::ECCommutativeCipher> ec_cipher)
    : ec_cipher_(std::move(ec_cipher)) {}

StatusOr<std::unique_ptr<PsiClient>> PsiClient::Create() {
  // Create an EC cipher with curve P-256. This gives 128 bits of security.
  ASSIGN_OR_RETURN(
      auto ec_cipher,
      ::private_join_and_compute::ECCommutativeCipher::CreateWithNewKey(
          NID_X9_62_prime256v1,
          ::private_join_and_compute::ECCommutativeCipher::HashType::SHA256));
  return absl::WrapUnique(new PsiClient(std::move(ec_cipher)));
}

StatusOr<std::string> PsiClient::CreateRequest(
    absl::Span<const std::string> inputs) const {
  // Encrypt inputs one by one.
  int64_t input_size = static_cast<int64_t>(inputs.size());
  std::vector<std::string> encrypted_inputs(input_size);
  for (int64_t i = 0; i < input_size; i++) {
    ASSIGN_OR_RETURN(encrypted_inputs[i], ec_cipher_->Encrypt(inputs[i]));
  }

  // Sort inputs (same effect as shuffling as they are encrypted) and store
  // them in a JSON array.
  rapidjson::Document request;
  request.SetArray();
  std::sort(encrypted_inputs.begin(), encrypted_inputs.end());
  for (int64_t i = 0; i < input_size; i++) {
    std::string base64_element = absl::Base64Escape(encrypted_inputs[i]);
    request.PushBack(rapidjson::Value().SetString(base64_element.data(),
                                                  base64_element.size(),
                                                  request.GetAllocator()),
                     request.GetAllocator());
  }

  // Return encrytped inputs as JSON array.
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  request.Accept(writer);
  return std::string(buffer.GetString());
}

StatusOr<int64_t> PsiClient::ProcessResponse(
    const std::string& server_setup, const std::string& server_response) const {
  // Parse setup and response message as JSON.
  rapidjson::Document setup, response;
  if (setup.Parse(server_setup.data(), server_setup.size()).HasParseError()) {
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

  // Decode Bloom filter from setup message.
  ASSIGN_OR_RETURN(auto bloom_filter,
                   BloomFilter::CreateFromJSON(server_setup));

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
    std::string base64_encrypted_element(value.GetString(),
                                         value.GetStringLength());
    std::string encrypted_element;
    if (!absl::Base64Unescape(base64_encrypted_element, &encrypted_element)) {
      return ::private_join_and_compute::InvalidArgumentError(
          "`server_response` elements must be valid Base64");
    }
    ASSIGN_OR_RETURN(std::string element,
                     ec_cipher_->Decrypt(encrypted_element));
    // Increase intersection size if element is found in the bloom filter.
    if (bloom_filter->Check(element)) {
      counter++;
    }
  }
  return counter;
}

}  // namespace private_set_intersection