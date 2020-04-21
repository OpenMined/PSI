// A Bloom filter succinctly encodes a set as an array of bits and can be used
// for approximate membership tests. For a given number of hash functions h_1,
// ..., h_k mapping strings to domain [m], an element x is inserted into the
// Bloom filter by setting bits h_1(x), ..., h_k(x) to 1. To test if an element
// was previously inserted, one just has to check if h_1(x) = ... = h_k(x) = 1.
// Due to collisions in the hash function values, Bloom filters come with an
// inherent false positive rate, which can be tuned by adjusting the number of
// hash functions k and the number of bits m in the Bloom filter. For a given
// false-positive rate e, we set m = -1.44 log2(e) * n, where n is the number of
// elements to be inserted, and k = -log2(e). See
// https://en.wikipedia.org/wiki/Bloom_filter#Optimal_number_of_hash_functions
// for more details.

#ifndef PSI_CARDINALITY_BLOOM_FILTER_H_
#define PSI_CARDINALITY_BLOOM_FILTER_H_

#include <vector>
#include "absl/types/span.h"
#include "crypto/context.h"
#include "util/statusor.h"

namespace psi_cardinality {

using ::private_join_and_compute::StatusOr;

class BloomFilter {
 public:
  // Creates a new Bloom filter. As long as less than `max_elements` are
  // inserted, the probability of false positives when performing checks against
  // the returned Bloom filter is less than `fpr`.
  static StatusOr<std::unique_ptr<BloomFilter>> Create(double fpr,
                                                       int64_t max_elements);

  // Adds `input` to the Bloom filter.
  void Add(const std::string& input);

  // Adds all elements in `inputs` to the Bloom filter.
  void Add(absl::Span<const std::string> inputs);

  // Checks if an element is present in the Bloom filter.
  bool Check(const std::string& input) const;

  // Returns a string representation of the Bloom filter.
  const std::string& ToString() const;

 private:
  BloomFilter() = delete;
  BloomFilter(double fpr, int num_hash_functions, std::string bits,
              std::unique_ptr<::private_join_and_compute::Context> context);

  // Hashes the input with all `num_hash_functions_` hash functions and returns
  // the result as a vector. The i-th hash  hash function is computed as
  // SHA256(i || x) % num_bits, where x is the input and num_bits is the number
  // of bits in the Bloom filter.
  std::vector<int64_t> Hash(const std::string& input) const;

  // False-positive rate.
  double fpr_;

  // Number of hash functions.
  int num_hash_functions_;

  // Compact representation of the bits. We use a std::string as opposed to
  // std::vector<bool> to allow serialization with ToString().
  std::string bits_;

  // OpenSSL context used for hashing.
  std::unique_ptr<::private_join_and_compute::Context> context_;
};

}  // namespace psi_cardinality

#endif  // PSI_CARDINALITY_BLOOM_FILTER_H_
