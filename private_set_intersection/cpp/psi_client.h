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

#include "absl/types/span.h"
#include "crypto/ec_commutative_cipher.h"
#include "util/statusor.h"

namespace private_set_intersection {

using ::private_join_and_compute::StatusOr;

// Client side of a Private Set Intersection-Cardinality protocol. In
// PSI-Cardinality, two parties (client and server) each hold a dataset, and at
// the end of the protocol the client learns the size of the intersection of
// both datasets, while no party learns anything beyond that.
//
// This variant of PSI-Cardinality introduces a small false-positive rate (i.e.,
// the reported cardinality will be slightly larger than the actual cardinality.
// The false positive rate can be tuned by the server.
//
// The protocol works as follows.
//
//
// 1. Setup phase
//
// The server encrypts all its elements x under a commutative encryption scheme,
// computing H(x)^s where s is its secret key. The encrypted elements are then
// inserted in a Bloom filter, which is sent to the client encoded as JSON. The
// message has the following form:
//
//   {
//     "num_hash_functions": <int>,
//     "bits": <string>
//   }
//
// Here, `bits` is a Base64-encoded string.
//
// 2. Client request
//
// The client encrypts all their elements x using the commutative encryption
// scheme, computing H(x)^c, where c is the client's secret key. The encoded
// elements are sent to the server as a JSON array of Base64 strings, together with a boolean reveal_intersection that indicates whether the client wants to learn the elements in the intersection or only its size.
//
//
//   {
//     "reveal_intersection": <bool>,
//     "encrypted_elements": [ Base64(H(x_1)^c), Base64(H(x_2)^c), ... ]
//   }
//
//
// 3. Server response
//
// For each encrypted element H(x)^c received from the client, the server
// encrypts it again under the commutative encryption scheme with its secret
// key s, computing (H(x)^c)^s = H(x)^(cs). The result is sent back to the
// client as a JSON array of strings:
//
//   {
//     "encrypted_elements": [ Base64(H(x_1)^c), Base64(H(x_2)^c), ... ]
//   }
//
// If reveal_intersection is false, the array is sorted to hide the order of
// entries from the client.
//
// 4. Client computes intersection
//
// The client decrypts each element received from the server's response using
// its secret key c, computing (H(x)^(cs))^(1/c) = H(x)^s. It then checks if
// each element is present in the Bloom filter, and reports the number of
// matches as the intersection size.
class PsiClient {
 public:
  PsiClient() = delete;

  // Creates and returns a new client instance.
  // If `reveal_intersection` is true, the client learns the elements in the
  // intersection of the two datasets. Otherwise, only the intersection size is
  // learned.
  //
  // Returns INTERNAL if any OpenSSL crypto operations fail.
  static StatusOr<std::unique_ptr<PsiClient>> Create(
      bool reveal_intersection);

  // Creates a request message to be sent to the server. For each input
  // element x, computes H(x)^c, where c is the secret key of ec_cipher_. The
  // result is sorted to hide the initial ordering of `inputs` and encoded as
  // a JSON array.
  //
  // Returns INTERNAL if encryption fails.
  StatusOr<std::string> CreateRequest(
      absl::Span<const std::string> inputs) const;

  // Processes the server's response and returns the intersection of the client
  // and server inputs. Use this function if this instance was created with
  // `reveal_intersectino = true`. The first argument, `server_setup`, is a
  // bloom filter that encodes encrypted server elements and is sent by the
  // server in a setup phase. The second argument, `server_response`, is the
  // response received from the server after sending the result of
  // `CreateRequest`.
  //
  // Returns INVALID_ARGUMENT if any input messages are malformed, or INTERNAL
  // if decryption fails.
  StatusOr<std::vector<int64_t>> GetIntersection(
      const std::string& server_setup,
      const std::string& server_response) const;

  // As `GetIntersection`, but only reveals the size of the intersection. Use
  // this function if this instance was created with `reveal_intersection =
  // false`.
  //
  // Returns INVALID_ARGUMENT if any input messages are malformed, or INTERNAL
  // if decryption fails.
  StatusOr<int64_t> GetIntersectionSize(
      const std::string& server_setup,
      const std::string& server_response) const;

 private:
  explicit PsiClient(
      std::unique_ptr<::private_join_and_compute::ECCommutativeCipher>
          ec_cipher,
      bool reveal_intersection);

  // Processes the `server_response` and returns the indices that are present in
  // the bloom filter encoded by `server_setup`. This method is called by
  // GetIntersection and GetIntersectionSize internally.
  StatusOr<std::vector<int64_t>> ProcessResponse(
      const std::string& server_setup,
      const std::string& server_response) const;

  std::unique_ptr<::private_join_and_compute::ECCommutativeCipher> ec_cipher_;
  bool reveal_intersection_;
};

}  // namespace private_set_intersection

#endif  // PRIVATE_SET_INTERSECTION_CPP_PSI_CLIENT_H_
