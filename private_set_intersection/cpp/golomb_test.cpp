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

#include "private_set_intersection/cpp/golomb.h"

#include "gtest/gtest.h"

namespace private_set_intersection {
namespace {

TEST(GolombTest, TestEncode) {
    char elements[] = { static_cast<char>(0b10000000) };
    std::string bloom_filter(elements, sizeof(elements) / sizeof(char));
    std::string encoded = golomb_compress(bloom_filter);
    char golomb[] = {2, static_cast<char>(0b00001110)}; // 7 / 4 = 1, 7 % 4 = 3
    std::string golomb_str(golomb, sizeof(golomb) / sizeof(char));
    EXPECT_EQ(encoded, golomb_str);
}

TEST(GolombTest, TestEncodeDecodeShort) {
    char elements[] = { static_cast<char>(0b10000000) };
    auto length = sizeof(elements) / sizeof(char);
    std::string bloom_filter(elements, length);
    std::string encoded = golomb_compress(bloom_filter);
    std::string decoded = golomb_decompress(encoded);
    EXPECT_EQ(bloom_filter, decoded);
}

TEST(GolombTest, TestEncodeDecodeLong) {
    char elements[] = { static_cast<char>(0b01001000), static_cast<char>(0b00100010), static_cast<char>(0b10000000) };
    auto length = sizeof(elements) / sizeof(char);
    std::string bloom_filter(elements, length);
    std::string encoded = golomb_compress(bloom_filter);
    std::string decoded = golomb_decompress(encoded);
    EXPECT_EQ(bloom_filter, decoded);
}

}  // namespace
}  // namespace private_set_intersection
