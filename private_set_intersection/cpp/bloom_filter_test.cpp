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

#include "private_set_intersection/cpp/bloom_filter.h"

#include "absl/strings/escaping.h"
#include "absl/strings/str_cat.h"
#include "gtest/gtest.h"
#include "private_set_intersection/proto/psi.pb.h"
#include "private_set_intersection/cpp/golomb.h"
#include "util/status_matchers.h"
#include <iostream>

namespace private_set_intersection {
namespace {

class BloomFilterTest : public ::testing::Test {
 protected:
  void SetUp() { return SetUp(0.001, 1 << 10); }
  void SetUp(double fpr, int max_elements, bool golomb = false) {
    PSI_ASSERT_OK_AND_ASSIGN(filter_, BloomFilter::Create(fpr, max_elements, golomb));
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
  EXPECT_EQ(encoded_filter.num_hash_functions(), filter_->NumHashFunctions());
  EXPECT_EQ(encoded_filter.num_hash_functions(), 7);
  EXPECT_EQ(encoded_filter.bits(), golomb_compress(filter_->Bits()).compressed);
  EXPECT_EQ(
      absl::Base64Escape(encoded_filter.bits()),
      "JEmlqqoSqFRIikJAqlREVEqhiKBSRBIkRFRSUaAgESlCSaRCKFShkJAgpFCVokIqRSIhUIqKEhVSqlSRVIVAKogkARFRqSqSqgQqUalEQVRUklRKKlWSilJEQpESRYhIBUpJRFVQSqWiqiIVkZIqSEWKiqqSqJRKKqqoUlUqKkiVoqCoUlBCqFKpCEkqRaASUVUqqhJFkYhCSqBKVFIoqipKSSUlqpBKEZFCUqhEkqiqqFIIJII=");
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

TEST_F(BloomFilterTest, TestGolombSmall) {
  std::vector<std::string> elements = {"a", "b", "c", "d"};
  filter_->Add(elements);
  auto encoded = golomb_compress(filter_->Bits());
  auto decoded = golomb_decompress(encoded.compressed, encoded.div, filter_->Bits().length());
  EXPECT_EQ(filter_->Bits(), decoded);
}

TEST_F(BloomFilterTest, TestGolombLarge) {
  double target_fpr = 0.000001;
  size_t max_elements = 1000;
  SetUp(target_fpr, max_elements, false);

  for (int i = 0; i < max_elements; i++) {
    filter_->Add(absl::StrCat("Element ", i));
  }

  std::cout << "Naive length (8 bytes per element): " << (max_elements * 8) << "\n";
  std::cout << "Bloom filter length: " << filter_->Bits().length() << "\n";
  auto encoded = golomb_compress(filter_->Bits(), -1);
  std::cout << "Compressed bloom filter length: " << encoded.compressed.length() << "\n";
  auto decoded = golomb_decompress(encoded.compressed, encoded.div, filter_->Bits().length());
  EXPECT_EQ(filter_->Bits(), decoded);
}

}  // namespace
}  // namespace private_set_intersection
