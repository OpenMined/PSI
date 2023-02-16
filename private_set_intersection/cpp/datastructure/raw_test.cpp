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

#include "absl/strings/escaping.h"
#include "absl/strings/str_cat.h"
#include "gtest/gtest.h"
#include "private_set_intersection/cpp/util/status_matchers.h"
#include "private_set_intersection/proto/psi.pb.h"

namespace private_set_intersection {
namespace {

class RawTest : public ::testing::Test {
 protected:
  void SetUp(int64_t num_client_inputs,
             std::vector<std::string> encrypted_elements) {
    PSI_ASSERT_OK_AND_ASSIGN(
        container_, Raw::Create(num_client_inputs, encrypted_elements));
  }

  std::unique_ptr<Raw> container_;
};

TEST_F(RawTest, TestIntersection) {
  std::vector<std::string> server = {"a", "b", "c", "d", "e"};
  std::vector<std::string> client = {"b", "c", "d", "z"};

  SetUp(static_cast<int64_t>(client.size()), server);
  std::vector<int64_t> results = container_->Intersect(absl::MakeSpan(client));
  std::vector<int64_t> expected{0, 1, 2};
  EXPECT_EQ(results.size(), 3);
  EXPECT_EQ(results, expected);
}

TEST_F(RawTest, TestIntersectionLargerClient) {
  std::vector<std::string> server = {"b", "c", "d", "z"};
  std::vector<std::string> client = {"a", "b", "c", "d", "e"};

  SetUp(static_cast<int64_t>(client.size()), server);
  std::vector<int64_t> results = container_->Intersect(absl::MakeSpan(client));
  std::vector<int64_t> expected{1, 2, 3};
  EXPECT_EQ(results.size(), 3);
  EXPECT_EQ(results, expected);
}

TEST_F(RawTest, TestToProtobuf) {
  std::vector<std::string> server = {"b", "a", "c", "d", "e"};
  std::vector<std::string> client = {"b", "c", "d", "z"};

  SetUp(static_cast<int64_t>(client.size()), server);

  // Create the protobuf from the Raw container and check if it matches.
  psi_proto::ServerSetup encoded_filter = container_->ToProtobuf();
  EXPECT_EQ(encoded_filter.raw().encrypted_elements().size(),
            container_->size());

  // In Raw mode, the encrypted results are sorted so this check
  // is to verify we're performing a sort correctly.
  EXPECT_EQ(encoded_filter.raw().encrypted_elements()[0], "a");
}

TEST_F(RawTest, TestIntersectionFromProtobuf) {
  std::vector<std::string> server = {"a", "b", "c", "d", "e"};
  std::vector<std::string> client = {"b", "c", "d", "z"};

  SetUp(static_cast<int64_t>(client.size()), server);

  // Create the protobuf from the Raw container and check if it matches.
  PSI_ASSERT_OK_AND_ASSIGN(auto container2,
                           Raw::CreateFromProtobuf(container_->ToProtobuf()));
  std::vector<int64_t> results = container2->Intersect(absl::MakeSpan(client));
  std::vector<int64_t> expected{0, 1, 2};
  EXPECT_EQ(results.size(), 3);
  EXPECT_EQ(results, expected);
}

}  // namespace
}  // namespace private_set_intersection
