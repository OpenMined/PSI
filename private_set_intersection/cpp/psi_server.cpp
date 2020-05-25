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

#include "private_set_intersection/cpp/psi_server.h"

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

PsiServer::PsiServer(
    std::unique_ptr<::private_join_and_compute::ECCommutativeCipher> ec_cipher, bool reveal_intersection)
    : ec_cipher_(std::move(ec_cipher)), reveal_intersection_(reveal_intersection) {}

StatusOr<std::unique_ptr<PsiServer>> PsiServer::CreateWithNewKey(bool reveal_intersection) {
  // Create an EC cipher with curve P-256. This gives 128 bits of security.
  ASSIGN_OR_RETURN(
      auto ec_cipher,
      ::private_join_and_compute::ECCommutativeCipher::CreateWithNewKey(
          NID_X9_62_prime256v1,
          ::private_join_and_compute::ECCommutativeCipher::HashType::SHA256));
  return absl::WrapUnique(new PsiServer(std::move(ec_cipher), reveal_intersection));
}

StatusOr<std::unique_ptr<PsiServer>> PsiServer::CreateFromKey(
    const std::string& key_bytes, bool reveal_intersection) {
  // Create an EC cipher with curve P-256. This gives 128 bits of security.
  ASSIGN_OR_RETURN(
      auto ec_cipher,
      ::private_join_and_compute::ECCommutativeCipher::CreateFromKey(
          NID_X9_62_prime256v1, key_bytes,
          ::private_join_and_compute::ECCommutativeCipher::HashType::SHA256));
  return absl::WrapUnique(new PsiServer(std::move(ec_cipher), reveal_intersection));
}

StatusOr<std::string> PsiServer::CreateSetupMessage(
    double fpr, int64_t num_client_inputs,
    absl::Span<const std::string> inputs) const {
  int64_t num_inputs = static_cast<int64_t>(inputs.size());
  // Correct fpr to account for multiple client queries.
  double corrected_fpr = fpr / num_client_inputs;

  // Create a Bloom filter and insert elements into it.
  ASSIGN_OR_RETURN(auto bloom_filter,
                   BloomFilter::Create(corrected_fpr, num_inputs));
  for (int i = 0; i < num_inputs; i++) {
    ASSIGN_OR_RETURN(std::string encrypted_element,
                     ec_cipher_->Encrypt(inputs[i]));
    bloom_filter->Add(encrypted_element);
  }

  // Encode Bloom filter as JSON and return it.
  return bloom_filter->ToJSON();
}

StatusOr<std::string> PsiServer::ProcessRequest(
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
  if (!request.IsObject()) {
    return ::private_join_and_compute::InvalidArgumentError(
        "`client_request` must be a JSON object");
  }
  bool client_wants_intersection = (request.HasMember("reveal_intersection") && request["reveal_intersection"].GetBool() == true);
  if (client_wants_intersection != reveal_intersection_) {
    return ::private_join_and_compute::InvalidArgumentError(
        absl::StrCat("Client expects `reveal_intersection` = ", client_wants_intersection, ", but it's actually ", reveal_intersection_, "."));
  }
  if (!request.HasMember("encrypted_elements")) {
    return ::private_join_and_compute::InvalidArgumentError(
        "`client_request` must contain member `encrytped_elements`");
  }

  // Re-encrypt elements.
  const auto encrypted_elements = request.GetArray();
  int64_t num_client_elements = static_cast<int64_t>(encrypted_elements.Size());
  std::vector<std::string> reencrypted_elements(num_client_elements);
  for (int i = 0; i < num_client_elements; i++) {
    if (!encrypted_elements[i].IsString()) {
      return ::private_join_and_compute::InvalidArgumentError(
          "`client_request` elements must be strings");
    }
    std::string base64_encrypted_element(encrypted_elements[i].GetString(),
                                         encrypted_elements[i].GetStringLength());
    std::string encrypted_element;
    if (!absl::Base64Unescape(base64_encrypted_element, &encrypted_element)) {
      return ::private_join_and_compute::InvalidArgumentError(
          "`client_request` elements must be valid Base64");
    }
    ASSIGN_OR_RETURN(reencrypted_elements[i],
                     ec_cipher_->ReEncrypt(encrypted_element));
  }

  // sort the resulting ciphertexts if we want to hide the intersection from the client.
  if (!reveal_intersection_) {
    std::sort(reencrypted_elements.begin(), reencrypted_elements.end());
  }

  // Encode re-encrypted elements as JSON.
  rapidjson::Document response;
  response.SetObject();
  rapidjson::Value response_elements;
  response_elements.SetArray();
  for (int i = 0; i < num_client_elements; i++) {
    std::string base64_element = absl::Base64Escape(reencrypted_elements[i]);
    response_elements.PushBack(rapidjson::Value().SetString(base64_element.data(),
                                                   base64_element.size(),
                                                   response.GetAllocator()),
                      response.GetAllocator());
  }
  response.AddMember("encrypted_elements", response_elements.Move(), response.GetAllocator());
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  response.Accept(writer);
  return std::string(buffer.GetString());
}

std::string PsiServer::GetPrivateKeyBytes() const {
  std::string key = ec_cipher_->GetPrivateKeyBytes();
  key.insert(key.begin(), 32 - key.length(), '\0');
  return key;
}

}  // namespace private_set_intersection