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

#include "absl/strings/str_cat.h"
#include "bloom_filter_c.h"
#include "gtest/gtest.h"
#include "util/status_matchers.h"

namespace psi_cardinality {
namespace {

class BloomFilterBindingTest : public ::testing::Test {
   protected:
    void SetUp() { return SetUp(0.001, 1 << 10); }
    void SetUp(double fpr, int max_elements) {
        filter_ = bloom_filter_create(fpr, max_elements);
        EXPECT_TRUE(filter_ != nullptr);
    }
    void TearDown() { bloom_filter_delete(filter_); }

    bloom_filter_ctx filter_;
};

TEST_F(BloomFilterBindingTest, TestAdd) {
    const char* elements[4] = {"a", "b", "c", "d"};

    // Test both variants of Add.
    bloom_filter_add(filter_, elements[0]);
    EXPECT_TRUE(bloom_filter_check(filter_, elements[0]));

    bloom_filter_add_array(filter_, elements, 4);

    // Check if all elements are present.
    for (size_t idx = 0; idx < 4; ++idx) {
        EXPECT_TRUE(bloom_filter_check(filter_, elements[idx]));
    }
    EXPECT_FALSE(bloom_filter_check(filter_, "not present"));
}

TEST_F(BloomFilterBindingTest, TestFPR) {
    for (int max_elements = 1 << 10; max_elements < (1 << 20);
         max_elements *= 2) {
        double target_fpr = 0.1;
        SetUp(target_fpr, max_elements);
        // Insert `max_elements` elements.
        for (int i = 0; i < max_elements; i++) {
            bloom_filter_add(filter_, absl::StrCat("Element ", i).c_str());
        }
        // Test 10k elements to measure FPR.
        double count = 0;
        int num_tests = 10000;
        for (int i = 0; i < num_tests; i++) {
            if (bloom_filter_check(filter_, absl::StrCat("Test ", i).c_str())) {
                count++;
            }
        }
        // Check if actual FPR matches the target FPR, allowing for 20% error.
        double actual_fpr = count / num_tests;
        EXPECT_LT(actual_fpr, 1.2 * target_fpr)
            << absl::StrCat("max_elements: ", max_elements);
    }
}

TEST_F(BloomFilterBindingTest, TestToJSON) {
    double fpr = 0.01;
    int max_elements = 100;
    SetUp(fpr, max_elements);
    for (int i = 0; i < max_elements; i++) {
        bloom_filter_add(filter_, absl::StrCat("Element ", i).c_str());
    }

    // Encode Bloom filter as JSON and check if it matches.
    char encoded_filter[4096] = {0};
    bloom_filter_to_json(filter_, encoded_filter, 4096);

    std::string expected =
        "{\"num_hash_functions\":7,\"bits\":\"VN3/"
        "BXfUjEDvJLcxCTepUCTXGQwlTax0xHiMohCNb45uShFsznK099RH0CFVIMn91Bdc7jLkXH"
        "Xr"
        "Xp1NimmZSDrYSj5sd/"
        "500nroNOdXbtd53u8cejPMGxbx7kR1E1zyO19mSkYLXq4xf7au5dFN0qhxqfLnjaCE\"}";
    EXPECT_EQ(encoded_filter, expected);
}

TEST_F(BloomFilterBindingTest, TestCreateFromJSON) {
    const char* elements[4] = {"a", "b", "c", "d"};
    bloom_filter_add_array(filter_, elements, 4);

    char encoded_filter[4096] = {0};
    bloom_filter_to_json(filter_, encoded_filter, 4096);

    auto filter2 = bloom_filter_create_from_json(encoded_filter);
    ASSERT_TRUE(filter2 != nullptr);

    for (size_t idx = 0; idx < 4; ++idx) {
        EXPECT_TRUE(bloom_filter_check(filter2, elements[idx]));
    }
    EXPECT_FALSE(bloom_filter_check(filter2, "not present"));
}
}  // namespace
}  // namespace psi_cardinality
