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

#include "private_set_intersection/cpp/datastructure/bloom_filter.h"

#include "absl/strings/escaping.h"
#include "absl/strings/str_cat.h"
#include "gtest/gtest.h"
#include "private_set_intersection/cpp/util/status_matchers.h"
#include "private_set_intersection/proto/psi.pb.h"

namespace private_set_intersection {
namespace {

class BloomFilterTest : public ::testing::Test {
 protected:
  void SetUp() { return SetUp(0.001, 1 << 10); }
  void SetUp(double fpr, int max_elements) {
    PSI_ASSERT_OK_AND_ASSIGN(filter_,
                             BloomFilter::CreateEmpty(fpr, max_elements));
  }

  std::unique_ptr<BloomFilter> filter_;
};

TEST_F(BloomFilterTest, TestAdd) {
  std::vector<std::string> elements = {"a", "b", "c", "d"};

  // Test both variants of Add.
  filter_->Add(elements[0]);
  filter_->Add(absl::MakeConstSpan(&elements[1], elements.size() - 1));

  // Check if all elements are present.
  for (const auto& element : elements) {
    EXPECT_TRUE(filter_->Check(element));
  }
  EXPECT_FALSE(filter_->Check("not present"));
}

TEST_F(BloomFilterTest, TestFPR) {
  for (int max_elements = 1 << 10; max_elements < (1 << 20);
       max_elements *= 2) {
    double target_fpr = 0.1;
    SetUp(target_fpr, max_elements);
    // Insert `max_elements` elements.
    for (int i = 0; i < max_elements; i++) {
      filter_->Add(absl::StrCat("Element ", i));
    }
    // Test 10k elements to measure FPR.
    double count = 0;
    int num_tests = 10000;
    for (int i = 0; i < num_tests; i++) {
      if (filter_->Check(absl::StrCat("Test ", i))) {
        count++;
      }
    }
    // Check if actual FPR matches the target FPR, allowing for 20% error.
    double actual_fpr = count / num_tests;
    EXPECT_LT(actual_fpr, 1.2 * target_fpr)
        << absl::StrCat("max_elements: ", max_elements);
  }
}

TEST_F(BloomFilterTest, TestToProtobuf) {
  double fpr = 0.01;
  int max_elements = 100;
  SetUp(fpr, max_elements);
  for (int i = 0; i < max_elements; i++) {
    filter_->Add(absl::StrCat("Element ", i));
  }

  // Create the protobuf from the Bloom filter and check if it matches.
  psi_proto::ServerSetup encoded_filter = filter_->ToProtobuf();
  EXPECT_EQ(encoded_filter.bloom_filter().num_hash_functions(),
            filter_->NumHashFunctions());
  EXPECT_EQ(encoded_filter.bloom_filter().num_hash_functions(), 7);
  EXPECT_EQ(encoded_filter.bloom_filter().bits(), filter_->Bits());
  EXPECT_EQ(
      absl::Base64Escape(encoded_filter.bloom_filter().bits()),
      "VN3/"
      "BXfUjEDvJLcxCTepUCTXGQwlTax0xHiMohCNb45uShFsznK099RH0CFVIMn91Bdc7jLkXHXr"
      "Xp1NimmZSDrYSj5sd/"
      "500nroNOdXbtd53u8cejPMGxbx7kR1E1zyO19mSkYLXq4xf7au5dFN0qhxqfLnjaCE");
}

TEST_F(BloomFilterTest, TestCreateFromProtobuf) {
  std::vector<std::string> elements = {"a", "b", "c", "d"};
  filter_->Add(elements);
  psi_proto::ServerSetup encoded_filter = filter_->ToProtobuf();
  PSI_ASSERT_OK_AND_ASSIGN(auto filter2,
                           BloomFilter::CreateFromProtobuf(encoded_filter));
  for (const auto& element : elements) {
    EXPECT_TRUE(filter2->Check(element));
  }
  EXPECT_FALSE(filter2->Check("not present"));
}

}  // namespace
}  // namespace private_set_intersection
