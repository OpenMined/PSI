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

#include "private_set_intersection/cpp/datastructure/gcs.h"

#include <iostream>

#include "absl/container/flat_hash_set.h"
#include "absl/strings/escaping.h"
#include "absl/strings/str_cat.h"
#include "gtest/gtest.h"
#include "private_set_intersection/cpp/util/status_matchers.h"
#include "private_set_intersection/proto/psi.pb.h"

namespace private_set_intersection {
namespace {

TEST(GCSTest, TestIntersect) {
  std::vector<std::string> elements = {"a", "b", "c", "d"};
  std::vector<std::string> elements2 = {"a", "b", "d", "e"};
  auto max_inputs =
      static_cast<int64_t>(std::max(elements.size(), elements2.size()));

  std::unique_ptr<GCS> gcs;
  PSI_ASSERT_OK_AND_ASSIGN(
      gcs, GCS::Create(0.001, max_inputs,
                       absl::MakeConstSpan(&elements[0], elements.size())));

  std::vector<int64_t> intersect = {0, 1, 2};

  auto res =
      gcs->Intersect(absl::MakeConstSpan(&elements2[0], elements2.size()));
  absl::flat_hash_set<int64_t> set(res.begin(), res.end());

  EXPECT_EQ(res.size(), intersect.size());

  for (int64_t i : intersect) {
    EXPECT_TRUE(set.contains(i));
  }
}

TEST(GCSTest, TestFPR) {
  for (int max_elements = 1 << 10; max_elements < (1 << 20);
       max_elements *= 2) {
    double target_fpr = 0.1;
    std::vector<std::string> elements;
    elements.reserve(max_elements);

    for (int i = 0; i < max_elements; i++) {
      elements.push_back(absl::StrCat("Element ", i));
    }

    std::unique_ptr<GCS> gcs;
    PSI_ASSERT_OK_AND_ASSIGN(
        gcs, GCS::Create(target_fpr, (int64_t)elements.size(),
                         absl::MakeConstSpan(&elements[0], elements.size())));

    // Test 10k elements to measure FPR.
    int num_tests = 10000;
    std::vector<std::string> elements2;
    elements2.reserve(num_tests);

    for (int i = 0; i < num_tests; i++) {
      elements2.push_back(absl::StrCat("Test ", i));
    }

    auto count =
        gcs->Intersect(absl::MakeConstSpan(&elements2[0], elements2.size()))
            .size();

    // Check if actual FPR matches the target FPR, allowing for 20% error.
    double actual_fpr = count / num_tests;
    EXPECT_LT(actual_fpr, 1.2 * target_fpr)
        << absl::StrCat("max_elements: ", max_elements);
  }
}

TEST(GCSTest, TestToProtobuf) {
  double fpr = 0.01;
  int max_elements = 100;
  std::vector<std::string> elements;
  elements.reserve(max_elements);

  for (int i = 0; i < max_elements; i++) {
    elements.push_back(absl::StrCat("Element ", i));
  }

  std::unique_ptr<GCS> gcs;
  PSI_ASSERT_OK_AND_ASSIGN(
      gcs, GCS::Create(fpr, (int64_t)elements.size(),
                       absl::MakeConstSpan(&elements[0], elements.size())));

  // Create the protobuf from the GCS and check if it matches.
  psi_proto::ServerSetup encoded_gcs = gcs->ToProtobuf();
  EXPECT_EQ(encoded_gcs.gcs().div(), gcs->Div());
  EXPECT_EQ(encoded_gcs.gcs().hash_range(), gcs->HashRange());
  EXPECT_EQ(encoded_gcs.gcs().bits(), gcs->Golomb());
}

TEST(GCSTest, TestCreateFromProtobuf) {
  std::vector<std::string> elements = {"a", "b", "c", "d"};
  std::vector<std::string> elements2 = {"a", "b", "c", "d", "not present"};
  auto max_inputs =
      static_cast<int64_t>(std::max(elements.size(), elements2.size()));

  std::unique_ptr<GCS> gcs;
  PSI_ASSERT_OK_AND_ASSIGN(
      gcs, GCS::Create(0.001, max_inputs,
                       absl::MakeConstSpan(&elements[0], elements.size())));

  psi_proto::ServerSetup encoded_gcs = gcs->ToProtobuf();
  std::unique_ptr<GCS> gcs2;
  PSI_ASSERT_OK_AND_ASSIGN(gcs2, GCS::CreateFromProtobuf(encoded_gcs));

  auto res =
      gcs2->Intersect(absl::MakeConstSpan(&elements2[0], elements2.size()));
  std::vector<int64_t> intersect = {0, 1, 2, 3};
  absl::flat_hash_set<int64_t> set(res.begin(), res.end());

  EXPECT_EQ(res.size(), intersect.size());

  for (int64_t i : intersect) {
    EXPECT_TRUE(set.contains(i));
  }
}

TEST(GCSTest, TestGolombSize) {
  double fpr[] = {1e-6, 1e-7, 1e-8, 1e-9, 1e-10, 1e-11, 1e-12};
  int max_elements = 10000;
  std::vector<std::string> elements;
  elements.reserve(max_elements);
  std::vector<int64_t> idx;
  idx.reserve(max_elements);

  for (int i = 0; i < max_elements; i++) {
    elements.push_back(absl::StrCat("Element ", i));
    idx.push_back(static_cast<int64_t>(i));
  }

  for (size_t i = 0; i < sizeof(fpr) / sizeof(double); i++) {
    std::unique_ptr<GCS> gcs;
    PSI_ASSERT_OK_AND_ASSIGN(
        gcs, GCS::Create(fpr[i], (int64_t)elements.size(),
                         absl::MakeConstSpan(&elements[0], elements.size())));
    auto res =
        gcs->Intersect(absl::MakeConstSpan(&elements[0], elements.size()));
    absl::flat_hash_set<int64_t> set(res.begin(), res.end());

    EXPECT_EQ(res.size(), idx.size());

    for (int64_t j : idx) {
      EXPECT_TRUE(set.contains(j));
    }

    std::cout << "Max elements: " << max_elements << ", FPR: " << fpr[i]
              << ", div: " << gcs->Div() << "\n";
    std::cout << "Naive length (8 bytes per element): " << (max_elements * 8)
              << "\n";
    auto filter_length = static_cast<int64_t>(
        std::ceil(-max_elements * std::log2(fpr[i]) / std::log(2) / 8));
    std::cout << "Bloom filter length: " << filter_length << "\n";
    std::cout << "GCS length: " << gcs->Golomb().length() << "\n";
  }
}

}  // namespace
}  // namespace private_set_intersection
