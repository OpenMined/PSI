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

#ifndef PRIVATE_SET_INTERSECTION_CPP_BLOOM_FILTER_H_
#define PRIVATE_SET_INTERSECTION_CPP_BLOOM_FILTER_H_

#include <vector>

#include "absl/status/statusor.h"
#include "absl/types/span.h"
#include "private_join_and_compute/crypto/context.h"
#include "private_set_intersection/proto/psi.pb.h"

namespace private_set_intersection {

using absl::StatusOr;

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
class BloomFilter {
 public:
  BloomFilter() = delete;

  static StatusOr<std::unique_ptr<BloomFilter>> Create(
      double fpr, int64_t num_client_inputs,
      absl::Span<const std::string> elements);

  // Creates a new Bloom filter. As long as less than `max_elements` are
  // inserted, the probability of false positives when performing checks
  // against the returned Bloom filter is less than `fpr`.
  //
  // Returns INVALID_ARGUMENT if fpr is not in (0,1) or max_elements is not
  // positive.
  static StatusOr<std::unique_ptr<BloomFilter>> CreateEmpty(
      double fpr, int64_t max_elements);

  // Creates a Bloom filter containing the bits of the passed protobuf, and the
  // given number of hash functions.
  //
  // Returns INVALID_ARGUMENT if `num_hash_functions` is not positive or if
  // `bits` is empty.
  static StatusOr<std::unique_ptr<BloomFilter>> CreateFromProtobuf(
      const psi_proto::ServerSetup& encoded_filter);

  std::vector<int64_t> Intersect(absl::Span<const std::string> elements) const;

  // Adds `input` to the Bloom filter.
  void Add(const std::string& input);

  // Adds all elements in `inputs` to the Bloom filter.
  void Add(absl::Span<const std::string> inputs);

  // Checks if an element is present in the Bloom filter.
  bool Check(const std::string& input) const;

  // Returns a protobuf representation of the Bloom filter
  psi_proto::ServerSetup ToProtobuf() const;

  // Returns the number of hash functions of the Bloom filter.
  int NumHashFunctions() const;

  // Returns the bit representation of the Bloom filter in its current state.
  std::string Bits() const;

 private:
  BloomFilter(int num_hash_functions, std::string bits,
              std::unique_ptr<::private_join_and_compute::Context> context);

  // Hashes the input with all `num_hash_functions_` hash functions and
  // returns the result as a vector. The i-th hash  hash function is computed
  // as SHA256(1 || x) + i * SHA256(2 || x) (modulo num_bits), where x is the
  // input and num_bits is the number of bits in the Bloom filter.
  std::vector<int64_t> Hash(const std::string& input) const;

  // Number of hash functions.
  int num_hash_functions_;

  // Compact representation of the bits. We use a std::string as opposed to
  // std::vector<bool> to allow serialization with ToString().
  std::string bits_;

  // OpenSSL context used for hashing.
  std::unique_ptr<::private_join_and_compute::Context> context_;
};

}  // namespace private_set_intersection

#endif  // PRIVATE_SET_INTERSECTION_CPP_BLOOM_FILTER_H_
