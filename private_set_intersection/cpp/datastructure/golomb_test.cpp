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

#include "private_set_intersection/cpp/datastructure/golomb.h"

#include <utility>
#include <vector>

#include "gtest/gtest.h"

namespace private_set_intersection {
namespace {

TEST(GolombTest, TestEncode) {
  std::vector<int64_t> elements = {7};
  auto encoded = golomb_compress(elements);
  char golomb[] = {static_cast<char>(0b00001110)};  // 7 / 4 = 1, 7 % 4 = 3
  std::string golomb_str(golomb, sizeof(golomb) / sizeof(char));
  EXPECT_EQ(encoded.compressed, golomb_str);
  EXPECT_EQ(encoded.div, 2);
}

TEST(GolombTest, TestIntersectShort) {
  std::vector<int64_t> elements = {6};
  auto encoded = golomb_compress(elements);
  std::vector<std::pair<int64_t, int64_t>> elements2 = {std::make_pair(5, 5),
                                                        std::make_pair(6, 6)};
  std::vector<int64_t> decoded =
      golomb_intersect(encoded.compressed, encoded.div, elements2);
  std::vector<int64_t> intersect = {6};
  EXPECT_EQ(intersect, decoded);
}

TEST(GolombTest, TestEncodeDecodeLong) {
  std::vector<int64_t> elements = {0, 1, 10, 100};
  auto encoded = golomb_compress(elements);
  std::vector<std::pair<int64_t, int64_t>> elements2 = {
      std::make_pair(0, 0), std::make_pair(100, 100),
      std::make_pair(1000, 1000)};
  std::vector<int64_t> decoded =
      golomb_intersect(encoded.compressed, encoded.div, elements2);
  std::vector<int64_t> intersect = {0, 100};
  EXPECT_EQ(intersect, decoded);
}

}  // namespace
}  // namespace private_set_intersection
