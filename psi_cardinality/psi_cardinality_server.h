// The server side of a Private Set Intersection Cardinality protocol.
// See the documentation in PSICardinalityClient for a full description of the
// protocol.

#ifndef PSI_CARDINALITY_PSI_CARDINALITY_SERVER_H_
#define PSI_CARDINALITY_PSI_CARDINALITY_SERVER_H_

#include "absl/types/span.h"
#include "crypto/ec_commutative_cipher.h"
#include "util/statusor.h"

namespace psi_cardinality {

using ::private_join_and_compute::StatusOr;

class PSICardinalityServer {
 public:
  PSICardinalityServer() = delete;

  // Creates and returns a new server instance with a fresh private key.
  //
  // Returns INTERNAL if any OpenSSL crypto operations fail.
  static StatusOr<std::unique_ptr<PSICardinalityServer>> CreateWithNewkey();

  // Creates and returns a new server instance with the provided private key.
  //
  // Returns INTERNAL if any OpenSSL crypto operations fail.
  static StatusOr<std::unique_ptr<PSICardinalityServer>> CreateFromKey(
      const std::string& key_bytes);

  // Creates a setup message from the server's dataset to be sent to the client.
  // The setup message is a JSON-encoded Bloom filter with the given
  // false-positive rate `fpr` containing H(x)^s for each element x in `inputs`,
  // where s is the server's secret key. The message has the following form:
  //
  //   {
  //     "num_hash_functions": <int>,
  //     "bits": <string>
  //   }
  //
  // Returns INTERNAL if encryption fails.
  StatusOr<std::string> CreateSetupMessage(
      double fpr, absl::Span<const std::string> inputs) const;

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
