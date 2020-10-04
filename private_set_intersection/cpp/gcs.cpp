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

#include "private_set_intersection/cpp/gcs.h"

#include <cmath>
#include <utility>
#include <algorithm>

#include "absl/memory/memory.h"
#include "absl/strings/escaping.h"
#include "private_set_intersection/proto/psi.pb.h"
#include "private_set_intersection/cpp/golomb.h"
#include "util/canonical_errors.h"

namespace private_set_intersection {

GCS::GCS(
    std::string golomb, int64_t div, int64_t hash_range,
    std::unique_ptr<::private_join_and_compute::Context> context)
    : golomb_(std::move(golomb)),
      div_(div),
      hash_range_(hash_range),
      context_(std::move(context)) {}

StatusOr<std::unique_ptr<GCS>> GCS::Create(
    double fpr, absl::Span<const std::string> elements) {
  if (fpr <= 0 || fpr >= 1) {
    return ::private_join_and_compute::InvalidArgumentError(
        "`fpr` must be in (0,1)");
  }

  auto hash_range = static_cast<int64_t>(elements.size() / fpr);
  std::vector<int64_t> hashes;
  hashes.reserve(elements.size());
  auto context = absl::make_unique<::private_join_and_compute::Context>();

  for (const std::string& element : elements) {
    hashes.push_back(Hash(element, hash_range, *context));
  }

  std::sort(hashes.begin(), hashes.end());
  auto compressed = golomb_compress(hashes);
  auto div = compressed.div;
  return absl::WrapUnique(new GCS(std::move(compressed.compressed),
                                  div,
                                  hash_range,
                                  std::move(context)));
}

StatusOr<std::unique_ptr<GCS>> GCS::CreateFromProtobuf(
    const psi_proto::ServerSetup& encoded_set) {
  if (!encoded_set.IsInitialized()) {
    return ::private_join_and_compute::InvalidArgumentError(
        "`ServerSetup` is corrupt!");
  }

  auto context = absl::make_unique<::private_join_and_compute::Context>();
  return absl::WrapUnique(new GCS(std::move(encoded_set.golomb()),
                                          static_cast<int64_t>(encoded_set.div()),
                                          encoded_set.hash_range(),
                                          std::move(context)));
}

std::vector<int64_t> GCS::Intersect(absl::Span<const std::string> elements, bool sort_idx) const {
  std::vector<std::pair<int64_t, int64_t>> hashes;
  hashes.reserve(elements.size());

  for (int64_t i = 0; i < elements.size(); i++) {
    hashes.emplace_back(Hash(elements[i], hash_range_, *context_), i);
  }

  std::sort(hashes.begin(), hashes.end(),
            [](const std::pair<int64_t, int64_t>& a, const std::pair<int64_t, int64_t>& b) {
              return a.first < b.first;
            });
  auto res = golomb_intersect(golomb_, div_, hashes);

  if (sort_idx) {
      std::sort(res.begin(), res.end());
  }

  return res;
}

psi_proto::ServerSetup GCS::ToProtobuf() const {
  psi_proto::ServerSetup server_setup;
  server_setup.set_golomb(golomb_);
  server_setup.set_div(static_cast<int32_t>(div_));
  server_setup.set_hash_range(hash_range_);
  return server_setup;
}

int64_t GCS::Div() const { return div_; }

int64_t GCS::HashRange() const { return hash_range_; }

std::string GCS::Golomb() const { return golomb_; }

int64_t GCS::Hash(const std::string& input, int64_t hash_range, ::private_join_and_compute::Context& context) {
  const auto bn_num_bits = context.CreateBigNum(hash_range);

  const int64_t h =
      context.CreateBigNum(context.Sha256String(input))
          .Mod(bn_num_bits)
          .ToIntValue()
          .ValueOrDie();  // ValueOrDie is safe here since bn_num_bits fits in
                          // an int64.
  return h;
}

}  // namespace private_set_intersection
