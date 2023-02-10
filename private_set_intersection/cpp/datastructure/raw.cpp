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

Raw::Raw(std::vector<std::string> elements) : encrypted_(std::move(elements)) {}

StatusOr<std::unique_ptr<Raw>> Raw::Create(int64_t num_client_inputs,
                                           std::vector<std::string> elements) {
  auto num_server_inputs = static_cast<int64_t>(elements.size());

  // If server inputs < client inputs, add random encrypted values
  // ...

  // Then we perform a sort to make intersections easier to find
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
  std::vector<int64_t> res;

  for (size_t i = 0; i < elements.size(); i++) {
    if (std::binary_search(encrypted_.begin(), encrypted_.end(), elements[i])) {
      res.push_back(i);
    }
  }

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
