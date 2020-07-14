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
#include "private_set_intersection/proto/psi.pb.h"
#include "util/canonical_errors.h"
#include "util/status_macros.h"

namespace private_set_intersection {

PsiServer::PsiServer(
    std::unique_ptr<::private_join_and_compute::ECCommutativeCipher> ec_cipher,
    bool reveal_intersection)
    : ec_cipher_(std::move(ec_cipher)),
      reveal_intersection(reveal_intersection) {}

StatusOr<std::unique_ptr<PsiServer>> PsiServer::CreateWithNewKey(
    bool reveal_intersection) {
  // Create an EC cipher with curve P-256. This gives 128 bits of security.
  ASSIGN_OR_RETURN(
      auto ec_cipher,
      ::private_join_and_compute::ECCommutativeCipher::CreateWithNewKey(
          NID_X9_62_prime256v1,
          ::private_join_and_compute::ECCommutativeCipher::HashType::SHA256));
  return absl::WrapUnique(
      new PsiServer(std::move(ec_cipher), reveal_intersection));
}

StatusOr<std::unique_ptr<PsiServer>> PsiServer::CreateFromKey(
    const std::string& key_bytes, bool reveal_intersection) {
  // Create an EC cipher with curve P-256. This gives 128 bits of security.
  ASSIGN_OR_RETURN(
      auto ec_cipher,
      ::private_join_and_compute::ECCommutativeCipher::CreateFromKey(
          NID_X9_62_prime256v1, key_bytes,
          ::private_join_and_compute::ECCommutativeCipher::HashType::SHA256));
  return absl::WrapUnique(
      new PsiServer(std::move(ec_cipher), reveal_intersection));
}

StatusOr<psi_proto::ServerSetup> PsiServer::CreateSetupMessage(
    double fpr, int64_t num_client_inputs,
    absl::Span<const std::string> inputs) const {
  auto num_inputs = static_cast<int64_t>(inputs.size());
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

  // Return the Bloom filter as a Protobuf
  return bloom_filter->ToProtobuf();
}

StatusOr<psi_proto::Response> PsiServer::ProcessRequest(
    const psi_proto::Request& client_request) const {
  if (!client_request.IsInitialized()) {
    return ::private_join_and_compute::InvalidArgumentError(
        "`client_request` is corrupt!");
  }

  if (client_request.reveal_intersection() != reveal_intersection) {
    return ::private_join_and_compute::InvalidArgumentError(
        absl::StrCat("Client expects `reveal_intersection` = ",
                     client_request.reveal_intersection(),
                     ", but it is actually ", reveal_intersection));
  }

  // Re-encrypt elements.
  const auto& encrypted_elements = client_request.encrypted_elements();
  const std::int64_t num_client_elements =
      static_cast<std::int64_t>(encrypted_elements.size());

  // Create the response
  psi_proto::Response response;

  // Re-encrypt the request's elements and add to the response
  for (int i = 0; i < num_client_elements; i++) {
    ASSIGN_OR_RETURN(std::string encrypted,
                     ec_cipher_->ReEncrypt(encrypted_elements[i]));
    response.add_encrypted_elements(encrypted);
  }

  // sort the resulting ciphertexts if we want to hide the intersection from the
  // client.
  if (!reveal_intersection) {
    // Get mutable reference to encrypted_elements array and sort it.
    auto& elements = *(response.mutable_encrypted_elements());
    std::sort(elements.begin(), elements.end());
  }
  return response;
}

std::string PsiServer::GetPrivateKeyBytes() const {
  std::string key = ec_cipher_->GetPrivateKeyBytes();
  key.insert(key.begin(), 32 - key.length(), '\0');
  return key;
}

}  // namespace private_set_intersection