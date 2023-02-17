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

#include "private_set_intersection/cpp/datastructure/raw.h"

#include <cmath>

#include "absl/memory/memory.h"
#include "absl/strings/escaping.h"
#include "absl/strings/str_cat.h"
#include "private_set_intersection/proto/psi.pb.h"

namespace private_set_intersection {

// Computes the intersection of two collections. The first collection must be a
// `pair<T, int64_t>`. The `T` must be the same in the second collection.
//
// Requires both collections to be sorted.
//
// Complexity:
// - O(max(n, m))
template <class InputIt1, class InputIt2, class OutputIt>
void custom_set_intersection(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                             InputIt2 last2, OutputIt d_first) {
  while (first1 != last1 && first2 != last2) {
    if ((*first1).first < *first2)
      ++first1;
    else {
      // *first1 and *first2 are equivalent.
      if (!(*first2 < (*first1).first)) {
        *d_first++ = (*first1++).second;
      }
      ++first2;
    }
  }
}

Raw::Raw(std::vector<std::string> elements) : encrypted_(std::move(elements)) {}

StatusOr<std::unique_ptr<Raw>> Raw::Create(int64_t num_client_inputs,
                                           std::vector<std::string> elements) {
  // We sort to make intersections easier to find later
  std::sort(elements.begin(), elements.end());

  return absl::WrapUnique(new Raw(elements));
}

StatusOr<std::unique_ptr<Raw>> Raw::CreateFromProtobuf(
    const psi_proto::ServerSetup& encoded_filter) {
  if (!encoded_filter.IsInitialized()) {
    return absl::InvalidArgumentError("`ServerSetup` is corrupt!");
  }

  std::vector<std::string> encrypted_elements(
      encoded_filter.raw().encrypted_elements().begin(),
      encoded_filter.raw().encrypted_elements().end());

  return absl::WrapUnique(new Raw(encrypted_elements));
}

std::vector<int64_t> Raw::Intersect(
    absl::Span<const std::string> elements) const {
  // This implementation creates a copy of `elements`, but the tradeoff is that
  // we can compute the intersection in O(nlog(n) + max(n, m)) where `n` and `m`
  // correspond to the number of client and server elements respectively.
  std::vector<std::pair<std::string, int64_t>> vp(elements.size());

  // Collect a pair with the index to track the original index after sorting.
  for (size_t i = 0; i < elements.size(); ++i) {
    vp[i] = make_pair(elements[i], (int64_t)i);
  }

  // Next, we sort the collection. O(nlog(n))
  std::sort(vp.begin(), vp.end());

  std::vector<int64_t> res;
  // Compute intersection. O(max(m, n))
  custom_set_intersection(vp.begin(), vp.end(), encrypted_.begin(),
                          encrypted_.end(), std::back_inserter(res));

  return res;
}

size_t Raw::size() const { return encrypted_.size(); }

psi_proto::ServerSetup Raw::ToProtobuf() const {
  psi_proto::ServerSetup server_setup;
  *server_setup.mutable_raw()->mutable_encrypted_elements() = {
      encrypted_.begin(), encrypted_.end()};

  return server_setup;
}

}  // namespace private_set_intersection
