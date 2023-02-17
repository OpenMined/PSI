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

#ifndef PRIVATE_SET_INTERSECTION_CPP_PSI_CLIENT_H_
#define PRIVATE_SET_INTERSECTION_CPP_PSI_CLIENT_H_

#include "absl/status/statusor.h"
#include "absl/types/span.h"
#include "private_join_and_compute/crypto/ec_commutative_cipher.h"
#include "private_set_intersection/proto/psi.pb.h"

namespace private_set_intersection {

using absl::StatusOr;

// Client side of a Private Set Intersection protocol. In PSI, two parties
// (client and server) each hold a dataset, and at the end of the protocol the
// client learns the size of the intersection of both datasets, while no party
// learns anything beyond that (cardinality mode).
//
// This container selected in this PSI library can introduce a small
// false-positive rate (i.e., the reported cardinality will be slightly larger
// than the actual cardinality. This false-positive rate can be tuned by the
// server.
//
// The protocol works as follows.
//
// 1. Setup phase
//
// The server encrypts all its elements x under a commutative encryption scheme,
// computing `H(x)^s` where `s` is its secret key. The encrypted elements are
// then inserted in a Bloom filter, which is sent to the client in the form of a
// serialized protobuf. The example `BloomFilter` container protobuf has the
// following form:
//
//   {
//     "num_hash_functions": <int>,
//     "bits": <string>
//   }
//
// Here, `bits` is a binary string.
//
// 2. Client request
//
// The client encrypts all their elements x using the commutative encryption
// scheme, computing H(x)^c, where c is the client's secret key. The encoded
// elements are sent to the server as an array together with a boolean
// reveal_intersection that indicates whether the client wants to learn the
// elements in the intersection or only its size. The payload is sent as a
// serialized protobuf to the client and holds the following form:
//
//
//   {
//     "reveal_intersection": <bool>,
//     "encrypted_elements": [ H(x_1)^c, H(x_2)^c, ... ]
//   }
//
//
// 3. Server response
//
// For each encrypted element `H(x)^c` received from the client, the server
// encrypts it again under the commutative encryption scheme with its secret key
// `s`, computing `(H(x)^c)^s = H(x)^(cs)`. The result is sent back to the
// client as a serialized protobuf holding the following form:
//
//   {
//     "encrypted_elements": [ H(x_1)^(cs), H(x_2)^(cs), ... ]
//   }
//
// If `reveal_intersection` is false, the array is sorted to hide the order of
// entries from the client.
//
// 4. Client computes intersection
//
// The client decrypts each element received from the server's response using
// its secret key `c`, computing `(H(x)^(cs))^(1/c) = H(x)^s`. It then checks if
// each element is present in the Bloom filter, and reports the number of
// matches as the intersection size.
class PsiClient {
 public:
  PsiClient() = delete;

  // Creates and returns a new client instance with a fresh private key. If
  // `reveal_intersection` is true, the client learns the elements in the
  // intersection of the two datasets. Otherwise, only the intersection size is
  // learned.
  //
  // Returns INTERNAL if any OpenSSL crypto operations fail.
  static StatusOr<std::unique_ptr<PsiClient>> CreateWithNewKey(
      bool reveal_intersection);

  // Creates and returns a new client instance with the provided private key. If
  // `reveal_intersection` is true, the client learns the elements in the
  // intersection of the two datasets. Otherwise, only the intersection size is
  // learned.
  //
  // WARNING: This function is provided for use in deterministic testing and
  // should be used with caution, since reusing the client key for multiple
  // requests can reveal information about the input sets. If in doubt, use
  // `CreateWithNewKey`.
  //
  // Returns INTERNAL if any OpenSSL crypto operations fail.
  static StatusOr<std::unique_ptr<PsiClient>> CreateFromKey(
      const std::string& key_bytes, bool reveal_intersection);

  // Creates a request protobuf to be serialized and sent to the server. For
  // each input element x, computes H(x)^c, where c is the secret key of
  // ec_cipher_.
  //
  // Returns INTERNAL if encryption fails.
  StatusOr<psi_proto::Request> CreateRequest(
      absl::Span<const std::string> inputs) const;

  // Processes the server's response and returns the intersection of the client
  // and server inputs. Use this function if this instance was created with
  // `reveal_intersection = true`. The first argument, `server_setup`, is a
  // bloom filter that encodes encrypted server elements and is sent by the
  // server in a setup phase. The second argument, `server_response`, is the
  // response received from the server after sending the result of
  // `CreateRequest`.
  //
  // Note that the intersections are returned in arbitrary order.
  //
  // Returns INVALID_ARGUMENT if any input messages are malformed, or INTERNAL
  // if decryption fails.
  StatusOr<std::vector<int64_t>> GetIntersection(
      const psi_proto::ServerSetup& server_setup,
      const psi_proto::Response& server_response) const;

  // As `GetIntersection`, but only reveals the size of the intersection. Use
  // this function if this instance was created with `reveal_intersection =
  // false`.
  //
  // Returns INVALID_ARGUMENT if any input messages are malformed, or INTERNAL
  // if decryption fails.
  StatusOr<int64_t> GetIntersectionSize(
      const psi_proto::ServerSetup& server_setup,
      const psi_proto::Response& server_response) const;

  // Returns this instance's private key. This key should only be used to create
  // other client instances. DO NOT SEND THIS KEY TO ANY OTHER PARTY!
  std::string GetPrivateKeyBytes() const;

 private:
  explicit PsiClient(
      std::unique_ptr<::private_join_and_compute::ECCommutativeCipher>
          ec_cipher,
      bool reveal_intersection);

  // Processes the `server_response` and returns the indices that are present in
  // the bloom filter encoded by `server_setup`. This method is called by
  // GetIntersection and GetIntersectionSize internally.
  StatusOr<std::vector<int64_t>> ProcessResponse(
      const psi_proto::ServerSetup& server_setup,
      const psi_proto::Response& server_response) const;

  std::unique_ptr<::private_join_and_compute::ECCommutativeCipher> ec_cipher_;
  bool reveal_intersection;
};

}  // namespace private_set_intersection

#endif  // PRIVATE_SET_INTERSECTION_CPP_PSI_CLIENT_H_
