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
#include "private_set_intersection/cpp/datastructure/bloom_filter.h"
#include "private_set_intersection/cpp/datastructure/gcs.h"
#include "private_set_intersection/cpp/datastructure/raw.h"
#include "private_set_intersection/proto/psi.pb.h"

namespace private_set_intersection {

/**
 * @brief Construct a new Psi Client:: Psi Client object
 *
 * @param ec_cipher A unique pointer to a commutative , which is used for
 * encryption and decryption in the Private Set Intersection (PSI) protocol.
 * @param reveal_intersection A boolean value indicating whether the
 * intersection of the two sets should be revealed after the PSI protocol is
 * completed.
 */
PsiClient::PsiClient(
    std::unique_ptr<::private_join_and_compute::ECCommutativeCipher> ec_cipher,
    bool reveal_intersection)
    : ec_cipher_(std::move(ec_cipher)),
      reveal_intersection(reveal_intersection) {}

/**
 * @brief Creates a new instance of the PsiClient class with a new key pair for
 * encryption and decryption using ECCommutativeCipher.
 *
 * @param reveal_intersection A boolean indicating whether the client wants to
 * learn the intersection values or only its size (cardinality).
 * @return StatusOr<std::unique_ptr<PsiClient>>
 */
StatusOr<std::unique_ptr<PsiClient>> PsiClient::CreateWithNewKey(
    bool reveal_intersection) {
  // Create an EC cipher with curve P-256. This gives 128 bits of security.
  ASSIGN_OR_RETURN(
      auto ec_cipher,
      ::private_join_and_compute::ECCommutativeCipher::CreateWithNewKey(
          NID_X9_62_prime256v1,
          ::private_join_and_compute::ECCommutativeCipher::HashType::SHA256));

  // Create a new instance of the PsiClient class using the ECCommutativeCipher
  // object and the reveal_intersection boolean.
  return absl::WrapUnique(
      new PsiClient(std::move(ec_cipher), reveal_intersection));
}

/**
 * @brief Creates a new PsiClient instance using an EC cipher created from the
 * provided key.
 *
 * @param key_bytes The bytes representing the key for the EC cipher.
 * @param reveal_intersection A boolean flag indicating whether the intersection
 * should be revealed.
 * @return StatusOr<std::unique_ptr<PsiClient>>
 */
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

/**
 * @brief Creates a request protobuf with encrypted inputs and a reveal flag.
 *
 * @param inputs The inputs to encrypt and add to the request protobuf.
 *
 * @return StatusOr<psi_proto::Request>
 */
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

/**
 * @brief Compute the intersection
 *
 * @param server_setup The original server's setup
 * @param server_response The previous server's response
 *
 * @return StatusOr<std::vector<int64_t>>
 */
StatusOr<std::vector<int64_t>> PsiClient::GetIntersection(
    const psi_proto::ServerSetup& server_setup,
    const psi_proto::Response& server_response) const {
  if (!reveal_intersection) {
    return absl::InvalidArgumentError(
        "GetIntersection called on PsiClient with reveal_intersection == "
        "false");
  }
  ASSIGN_OR_RETURN(std::vector<int64_t> intersection,
                   ProcessResponse(server_setup, server_response));
  intersection.shrink_to_fit();
  return intersection;
}

/**
 * @brief Compute the intersection (cardinality)
 *
 * @param server_setup The original server's setup
 * @param server_response The previous server's response
 *
 * @return StatusOr<int64_t>
 */
StatusOr<int64_t> PsiClient::GetIntersectionSize(
    const psi_proto::ServerSetup& server_setup,
    const psi_proto::Response& server_response) const {
  ASSIGN_OR_RETURN(std::vector<int64_t> intersection,
                   ProcessResponse(server_setup, server_response));
  return static_cast<int64_t>(intersection.size());
}

/**
 * @brief Process the server's response to obtain the intersection
 *
 * @param server_setup The original server's setup
 * @param server_response The previous server's response
 *
 * @return StatusOr<std::vector<int64_t>>
 */
StatusOr<std::vector<int64_t>> PsiClient::ProcessResponse(
    const psi_proto::ServerSetup& server_setup,
    const psi_proto::Response& server_response) const {
  // Ensure both items are valid
  if (!server_setup.IsInitialized()) {
    return absl::InvalidArgumentError("`server_setup` is corrupt!");
  }

  if (!server_response.IsInitialized()) {
    return absl::InvalidArgumentError("`server_response` is corrupt!");
  }

  const auto& response_array = server_response.encrypted_elements();
  const std::int64_t response_size =
      static_cast<std::int64_t>(response_array.size());
  std::vector<std::string> decrypted;
  decrypted.reserve(response_size);

  for (int64_t i = 0; i < response_size; i++) {
    ASSIGN_OR_RETURN(std::string element,
                     ec_cipher_->Decrypt(response_array[i]));
    decrypted.push_back(element);
  }

  switch (server_setup.data_structure_case()) {
    case psi_proto::ServerSetup::DataStructureCase::kRaw: {
      // Decode Bloom Filter from the server setup.
      ASSIGN_OR_RETURN(auto container, Raw::CreateFromProtobuf(server_setup));
      return container->Intersect(absl::MakeConstSpan(decrypted));
    }
    case psi_proto::ServerSetup::DataStructureCase::kGcs: {
      // Decode GCS from the server setup.
      ASSIGN_OR_RETURN(auto container, GCS::CreateFromProtobuf(server_setup));
      return container->Intersect(absl::MakeConstSpan(decrypted));
    }
    case psi_proto::ServerSetup::DataStructureCase::kBloomFilter: {
      // Decode Bloom Filter from the server setup.
      ASSIGN_OR_RETURN(auto container,
                       BloomFilter::CreateFromProtobuf(server_setup));
      return container->Intersect(absl::MakeConstSpan(decrypted));
    }
    default: {
      return absl::InvalidArgumentError("Impossible");
    }
  }
}

/**
 * @brief Get the client's private key
 *
 * @return The private key as a null-terminated binary string
 */
std::string PsiClient::GetPrivateKeyBytes() const {
  std::string key = ec_cipher_->GetPrivateKeyBytes();
  key.insert(key.begin(), 32 - key.length(), '\0');
  return key;
}

}  // namespace private_set_intersection
