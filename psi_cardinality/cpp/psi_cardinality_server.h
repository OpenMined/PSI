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

#ifndef PSI_CARDINALITY_PSI_CARDINALITY_SERVER_H_
#define PSI_CARDINALITY_PSI_CARDINALITY_SERVER_H_

#include "absl/types/span.h"
#include "crypto/ec_commutative_cipher.h"
#include "util/statusor.h"

namespace psi_cardinality {

using ::private_join_and_compute::StatusOr;

// The server side of a Private Set Intersection Cardinality protocol.
// See the documentation in PSICardinalityClient for a full description of the
// protocol.
class PSICardinalityServer {
 public:
  PSICardinalityServer() = delete;

  // Creates and returns a new server instance with a fresh private key.
  //
  // Returns INTERNAL if any OpenSSL crypto operations fail.
  static StatusOr<std::unique_ptr<PSICardinalityServer>> CreateWithNewKey();

  // Creates and returns a new server instance with the provided private key.
  //
  // Returns INTERNAL if any OpenSSL crypto operations fail.
  static StatusOr<std::unique_ptr<PSICardinalityServer>> CreateFromKey(
      const std::string& key_bytes);

  // Creates a setup message from the server's dataset to be sent to the client.
  // The setup message is a JSON-encoded Bloom filter containing H(x)^s for each
  // element x in `inputs`, where s is the server's secret key. The message has
  // the following form:
  //
  //   {
  //     "num_hash_functions": <int>,
  //     "bits": <string>
  //   }
  //
  // `bits` is encoded as Base64.
  // The false-positive rate `fpr` is the probability that any query of size
  // `num_client_inputs` will result in a false positive.
  //
  // Returns INTERNAL if encryption fails.
  StatusOr<std::string> CreateSetupMessage(
      double fpr, int64_t num_client_inputs,
      absl::Span<const std::string> inputs) const;

  // Processes a client query and returns the corresponding server response to
  // be sent to the client. For each encrytped element H(x)^c in the decoded
  // `client_request`, computes (H(x)^c)^s = H(X)^(cs) and returns these as a
  // sorted JSON array. Sorting the output prevents the client from matching the
  // individual response elements to the ones in the request, ensuring that they
  // can only learn the intersection size but not individual elements in the
  // intersection.
  StatusOr<std::string> ProcessRequest(const std::string& client_request) const;

  // Returns this instance's private key. This key should only be used to create
  // other server instances. DO NOT SEND THIS KEY TO ANY OTHER PARTY!
  std::string GetPrivateKeyBytes() const;

 private:
  explicit PSICardinalityServer(
      std::unique_ptr<::private_join_and_compute::ECCommutativeCipher>
          ec_cipher);

  std::unique_ptr<::private_join_and_compute::ECCommutativeCipher> ec_cipher_;
};

}  // namespace psi_cardinality

#endif  // PSI_CARDINALITY_PSI_CARDINALITY_SERVER_H_
