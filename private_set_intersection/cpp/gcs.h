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

#ifndef PRIVATE_SET_INTERSECTION_CPP_GCS_H_
#define PRIVATE_SET_INTERSECTION_CPP_GCS_H_

#include <vector>

#include "absl/status/statusor.h"
#include "absl/types/span.h"
#include "private_join_and_compute/crypto/context.h"
#include "private_set_intersection/proto/psi.pb.h"

namespace private_set_intersection {

using absl::StatusOr;

class GCS {
 public:
  GCS() = delete;

  static StatusOr<std::unique_ptr<GCS>> Create(
      double fpr, int64_t num_client_inputs,
      absl::Span<const std::string> elements);

  static StatusOr<std::unique_ptr<GCS>> CreateFromProtobuf(
      const psi_proto::ServerSetup& encoded_set);

  std::vector<int64_t> Intersect(absl::Span<const std::string> elements) const;

  psi_proto::ServerSetup ToProtobuf() const;

  int64_t Div() const;

  int64_t HashRange() const;

  std::string Golomb() const;

 private:
  GCS(std::string golomb, int64_t div, int64_t hash_range,
      std::unique_ptr<::private_join_and_compute::Context> context);

  static int64_t Hash(const std::string& input, int64_t hash_range,
                      ::private_join_and_compute::Context& context);

  std::string golomb_;

  int64_t div_;

  int64_t hash_range_;

  std::unique_ptr<::private_join_and_compute::Context> context_;
};

}  // namespace private_set_intersection

#endif  // PRIVATE_SET_INTERSECTION_CPP_GCS_H_
