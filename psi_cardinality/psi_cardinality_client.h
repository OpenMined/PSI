// Client side of a Private Set Intersection-Cardinality protocol. In
// PSI-Cardinality, two parties (Client and Server) each hold a dataset, and at
// the end of the protocol the Client learns the size of the intersection of
// both datasets, while no party learns anything beyond that.
//
// This variant of PSI-Cardinality introduces a small false-positive rate (i.e.,
// the reported cardinality will be slightly larger than the actual cardinality.
// The false positive rate can be tuned by the server.

#ifndef PSI_CARDINALITY_PSI_CARDINALITY_CLIENT_H_
#define PSI_CARDINALITY_PSI_CARDINALITY_CLIENT_H_

#include "absl/types/span.h"
#include "crypto/ec_commutative_cipher.h"
#include "util/statusor.h"

namespace psi_cardinality {

class PSICardinalityClient {
 public:
  // Creates and returns a new client instance.
  // Returns INTERNAL if any OpenSSL crypto operations fail.
  ::private_join_and_compute::StatusOr<std::unique_ptr<PSICardinalityClient>>
  Create();

  // Creates a request message to be sent to the server.
  // Returns INTERNAL if encryption fails.
  ::private_join_and_compute::StatusOr<std::string> CreateRequest(
      absl::Span<const std::string> inputs);

  // Processes the server's response and returns the PSI cardinality. The first
  // argument, `server_setup`, is a bloom filter that encodes encrypted server
  // elements and is sent by the server in a setup phase. The second argument,
  // `server_response`, is the response received from the server after sending
  // the result of `CreateRequest`. Returns INTERNAL if decryption fails or if
  // any of the input messages are malformed.
  ::private_join_and_compute::StatusOr<int64_t> ProcessResponse(
      const std::string& server_setup, const std::string& server_response);

 private:
  PSICardinalityClient() = delete;
  PSICardinalityClient(
      std::unique_ptr<::private_join_and_compute::ECCommutativeCipher>
          ec_cipher);

  std::unique_ptr<::private_join_and_compute::ECCommutativeCipher> ec_cipher_;
};

}  // namespace psi_cardinality

#endif  // PSI_CARDINALITY_PSI_CARDINALITY_CLIENT_H_
