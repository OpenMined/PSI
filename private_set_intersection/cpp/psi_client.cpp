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
#include "private_set_intersection/proto/psi.pb.h"
#include "util/canonical_errors.h"
#include "util/status_macros.h"

namespace private_set_intersection {

PsiClient::PsiClient(
    std::unique_ptr<::private_join_and_compute::ECCommutativeCipher> ec_cipher,
    bool reveal_intersection)
    : ec_cipher_(std::move(ec_cipher)),
      reveal_intersection(reveal_intersection) {}

StatusOr<std::unique_ptr<PsiClient>> PsiClient::CreateWithNewKey(
    bool reveal_intersection) {
  // Create an EC cipher with curve P-256. This gives 128 bits of security.
  ASSIGN_OR_RETURN(
      auto ec_cipher,
      ::private_join_and_compute::ECCommutativeCipher::CreateWithNewKey(
          NID_X9_62_prime256v1,
          ::private_join_and_compute::ECCommutativeCipher::HashType::SHA256));
  return absl::WrapUnique(
      new PsiClient(std::move(ec_cipher), reveal_intersection));
}

StatusOr<std::unique_ptr<PsiClient>> PsiClient::CreateFromKey(
    const std::string& key_bytes, bool reveal_intersection) {
  // Create an EC cipher with curve P-256. This gives 128 bits of security.
  ASSIGN_OR_RETURN(
      auto ec_cipher,
      ::private_join_and_compute::ECCommutativeCipher::CreateFromKey(
          NID_X9_62_prime256v1, key_bytes,
          ::private_join_and_compute::ECCommutativeCipher::HashType::SHA256));
  return absl::WrapUnique(
      new PsiClient(std::move(ec_cipher), reveal_intersection));
}

StatusOr<psi_proto::Request> PsiClient::CreateRequest(
    absl::Span<const std::string> inputs) const {
  // Encrypt inputs one by one.
  int64_t input_size = static_cast<int64_t>(inputs.size());
  std::vector<std::string> encrypted_inputs(input_size);
  for (int64_t i = 0; i < input_size; i++) {
    ASSIGN_OR_RETURN(encrypted_inputs[i], ec_cipher_->Encrypt(inputs[i]));
  }

  // Create a request protobuf
  psi_proto::Request request;

  // Set the reveal flag
  request.set_reveal_intersection(reveal_intersection);

  // Add the encrypted elements
  for (int64_t i = 0; i < input_size; i++) {
    request.add_encrypted_elements(encrypted_inputs[i]);
  }

  return request;
}

StatusOr<std::vector<int64_t>> PsiClient::GetIntersection(
    const psi_proto::ServerSetup& server_setup,
    const psi_proto::Response& server_response) const {
  if (!reveal_intersection) {
    return ::private_join_and_compute::InvalidArgumentError(
        "GetIntersection called on PsiClient with reveal_intersection == "
        "false");
  }
  ASSIGN_OR_RETURN(std::vector<int64_t> intersection,
                   ProcessResponse(server_setup, server_response));
  intersection.shrink_to_fit();
  return intersection;
}

StatusOr<int64_t> PsiClient::GetIntersectionSize(
    const psi_proto::ServerSetup& server_setup,
    const psi_proto::Response& server_response) const {
  ASSIGN_OR_RETURN(std::vector<int64_t> intersection,
                   ProcessResponse(server_setup, server_response));
  return static_cast<int64_t>(intersection.size());
}

StatusOr<std::vector<int64_t>> PsiClient::ProcessResponse(
    const psi_proto::ServerSetup& server_setup,
    const psi_proto::Response& server_response) const {
  // Ensure both items are valid
  if (!server_setup.IsInitialized()) {
    return ::private_join_and_compute::InvalidArgumentError(
        "`server_setup` is corrupt!");
  }

  if (!server_response.IsInitialized()) {
    return ::private_join_and_compute::InvalidArgumentError(
        "`server_response` is corrupt!");
  }

  // Decode Bloom filter from the server setup.
  ASSIGN_OR_RETURN(auto bloom_filter,
                   BloomFilter::CreateFromProtobuf(server_setup));

  const auto& response_array = server_response.encrypted_elements();
  const std::int64_t response_size =
      static_cast<std::int64_t>(response_array.size());
  std::vector<int64_t> result(0);
  result.reserve(response_size);

  // Decrypt and check if the element is in our filter
  for (int64_t i = 0; i < response_size; i++) {
    ASSIGN_OR_RETURN(std::string element,
                     ec_cipher_->Decrypt(response_array[i]));
    // Increase intersection size if element is found in the bloom filter.
    if (bloom_filter->Check(element)) {
      result.push_back(i);
    }
  }
  return result;
}

std::string PsiClient::GetPrivateKeyBytes() const {
  std::string key = ec_cipher_->GetPrivateKeyBytes();
  key.insert(key.begin(), 32 - key.length(), '\0');
  return key;
}

}  // namespace private_set_intersection