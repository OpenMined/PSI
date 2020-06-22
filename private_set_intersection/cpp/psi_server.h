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

#ifndef PRIVATE_SET_INTERSECTION_CPP_PSI_SERVER_H_
#define PRIVATE_SET_INTERSECTION_CPP_PSI_SERVER_H_

#include "absl/types/span.h"
#include "crypto/ec_commutative_cipher.h"
#include "private_set_intersection/proto/psi.pb.h"
#include "util/statusor.h"

namespace private_set_intersection {

using ::private_join_and_compute::StatusOr;

// The server side of a Private Set Intersection protocol.
// See the documentation in PsiClient for a full description of the
// protocol.
class PsiServer {
 public:
  PsiServer() = delete;

  // Creates and returns a new server instance with a fresh private key.
  // If `reveal_intersection` indicates whether the client should learn the
  // intersection or only its size.
  //
  // Returns INTERNAL if any OpenSSL crypto operations fail.
  static StatusOr<std::unique_ptr<PsiServer>> CreateWithNewKey(
      bool reveal_intersection);

  // Creates and returns a new server instance with the provided private key.
  // If `reveal_intersection` indicates whether the client should learn the
  // intersection or only its size.
  //
  // Returns INTERNAL if any OpenSSL crypto operations fail.
  static StatusOr<std::unique_ptr<PsiServer>> CreateFromKey(
      const std::string& key_bytes, bool reveal_intersection);

  // Creates a setup message from the server's dataset to be sent to the
  // client. The setup message is a Bloom filter containing
  // H(x)^s for each element x in `inputs`, where s is the server's secret
  // key. The setup is sent to the client as a serialized protobuf with 
  // the following form:
  //
  //   {
  //     "num_hash_functions": <int>,
  //     "bits": <string>
  //   }
  //
  // `bits` is a binary string.
  // The false-positive rate `fpr` is the probability that any query of size
  // `num_client_inputs` will result in a false positive.
  //
  // Returns INTERNAL if encryption fails.
  StatusOr<psi_proto::ServerSetup> CreateSetupMessage(
      double fpr, int64_t num_client_inputs,
      absl::Span<const std::string> inputs) const;

  // Processes a client query and returns the corresponding server response to
  // be sent to the client. For each encrytped element H(x)^c in the decoded
  // `client_request`, computes (H(x)^c)^s = H(X)^(cs) and returns these as an
  // array inside a protobuf.
  // If reveal_intersection == false, the resulting array is sorted, which
  // prevents the client from matching the individual response elements to the
  // ones in the request, ensuring that they can only learn the intersection
  // size but not individual elements in the intersection.
  //
  // Returns INVALID_ARGUMENT if the request is malformed or if
  // reveal_intersection != client_request["reveal_intersection"].
  StatusOr<psi_proto::Response> ProcessRequest(
      const psi_proto::Request& client_request) const;

  // Returns this instance's private key. This key should only be used to
  // create other server instances. DO NOT SEND THIS KEY TO ANY OTHER PARTY!
  std::string GetPrivateKeyBytes() const;

 private:
  explicit PsiServer(
      std::unique_ptr<::private_join_and_compute::ECCommutativeCipher>
          ec_cipher,
      bool reveal_intersection);

  std::unique_ptr<::private_join_and_compute::ECCommutativeCipher> ec_cipher_;
  bool reveal_intersection;
};

}  // namespace private_set_intersection

#endif  // PRIVATE_SET_INTERSECTION_CPP_PSI_SERVER_H_
