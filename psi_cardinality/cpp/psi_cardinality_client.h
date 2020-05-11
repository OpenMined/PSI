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

#ifndef PSI_CARDINALITY_PSI_CARDINALITY_CLIENT_H_
#define PSI_CARDINALITY_PSI_CARDINALITY_CLIENT_H_

#include "absl/types/span.h"
#include "crypto/ec_commutative_cipher.h"
#include "util/statusor.h"

namespace psi_cardinality {

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
// elements are sent to the server as a JSON array of Base64 strings in sorted
// order:
//
//   sort([ Base64(H(x_1)^c), Base64(H(x_2)^c), ... ])
//
//
// 3. Server response
//
// For each encrypted element H(x)^c received from the client, the server
// encrypts it again under the commutative encryption scheme with its secret
// key s, computing (H(x)^c)^s = H(x)^(cs). The result is sent back to the
// client as a JSON array of strings in sorted order:
//
//   sort([ Base64(H(x_1)^(cs)), Base64(H(x_2)^(cs)), ... ])
//
//
// 4. Client computes intersection
//
// The client decrypts each element received from the server's response using
// its secret key c, computing (H(x)^(cs))^(1/c) = H(x)^s. It then checks if
// each element is present in the Bloom filter, and reports the number of
// matches as the intersection size.
class PSICardinalityClient {
 public:
  PSICardinalityClient() = delete;

  // Creates and returns a new client instance.
  //
  // Returns INTERNAL if any OpenSSL crypto operations fail.
  static StatusOr<std::unique_ptr<PSICardinalityClient>> Create();

  // Creates a request message to be sent to the server. For each input
  // element x, computes H(x)^c, where c is the secret key of ec_cipher_. The
  // result is sorted to hide the initial ordering of `inputs` and encoded as
  // a JSON array.
  //
  // Returns INTERNAL if encryption fails.
  StatusOr<std::string> CreateRequest(
      absl::Span<const std::string> inputs) const;

  // Processes the server's response and returns the PSI cardinality. The
  // first argument, `server_setup`, is a bloom filter that encodes encrypted
  // server elements and is sent by the server in a setup phase. The second
  // argument, `server_response`, is the response received from the server
  // after sending the result of `CreateRequest`.
  //
  // Returns INVALID_ARGUMENT if any input messages are malformed, or INTERNAL
  // if decryption fails.
  StatusOr<int64_t> ProcessResponse(const std::string& server_setup,
                                    const std::string& server_response) const;

 private:
  explicit PSICardinalityClient(
      std::unique_ptr<::private_join_and_compute::ECCommutativeCipher>
          ec_cipher);

  std::unique_ptr<::private_join_and_compute::ECCommutativeCipher> ec_cipher_;
};

}  // namespace psi_cardinality

#endif  // PSI_CARDINALITY_PSI_CARDINALITY_CLIENT_H_
