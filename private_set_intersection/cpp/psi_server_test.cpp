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

#include "private_set_intersection/cpp/psi_server.h"

#include "absl/container/flat_hash_set.h"
#include "absl/strings/str_cat.h"
#include "crypto/ec_commutative_cipher.h"
#include "gtest/gtest.h"
#include "private_set_intersection/cpp/psi_client.h"
#include "rapidjson/document.h"
#include "util/status_matchers.h"

namespace private_set_intersection {
namespace {

class PsiServerTest : public ::testing::Test {
 protected:
  void SetUp(bool reveal_intersection) {
    PSI_ASSERT_OK_AND_ASSIGN(server_,
                             PsiServer::CreateWithNewKey(reveal_intersection));
  }

  std::unique_ptr<PsiServer> server_;
};

TEST_F(PsiServerTest, TestCorrectnessIntersection) {
  SetUp(true);
  // We use an actual client instance here, since we already test the client
  // on its own in psi_client_test.cpp.
  PSI_ASSERT_OK_AND_ASSIGN(auto client, PsiClient::Create(true));
  int num_client_elements = 1000, num_server_elements = 10000;
  double fpr = 0.01;
  std::vector<std::string> client_elements(num_client_elements);
  std::vector<std::string> server_elements(num_server_elements);

  // Create elements, such that 50% of the client's elements overlap with the
  // server.
  for (int i = 0; i < num_client_elements; i++) {
    client_elements[i] = absl::StrCat("Element ", i);
  }
  for (int i = 0; i < num_server_elements; i++) {
    server_elements[i] = absl::StrCat("Element ", 2 * i);
  }

  // Run Server setup.
  PSI_ASSERT_OK_AND_ASSIGN(
      auto server_setup,
      server_->CreateSetupMessage(fpr, num_client_elements, server_elements));

  // Create Client request.
  PSI_ASSERT_OK_AND_ASSIGN(auto client_request,
                           client->CreateRequest(client_elements));

  // Create Server response.
  PSI_ASSERT_OK_AND_ASSIGN(auto server_response,
                           server_->ProcessRequest(client_request));

  // Compute intersection.
  PSI_ASSERT_OK_AND_ASSIGN(
      std::vector<int64_t> intersection,
      client->GetIntersection(server_setup, server_response));
  absl::flat_hash_set<int64_t> intersection_set(intersection.begin(),
                                                intersection.end());

  // Test if all even elements are present.
  for (int i = 0; i < num_client_elements; i++) {
    if (i % 2) {
      EXPECT_FALSE(intersection_set.contains(i));
    } else {
      EXPECT_TRUE(intersection_set.contains(i));
    }
  }
}

TEST_F(PsiServerTest, TestCorrectnessIntersectionSize) {
  SetUp(false);
  // We use an actual client instance here, since we already test the client
  // on its own in psi_client_test.cpp.
  PSI_ASSERT_OK_AND_ASSIGN(auto client, PsiClient::Create(false));
  int num_client_elements = 1000, num_server_elements = 10000;
  double fpr = 0.01;
  std::vector<std::string> client_elements(num_client_elements);
  std::vector<std::string> server_elements(num_server_elements);

  // Create elements, such that 50% of the client's elements overlap with the
  // server.
  for (int i = 0; i < num_client_elements; i++) {
    client_elements[i] = absl::StrCat("Element ", i);
  }
  for (int i = 0; i < num_server_elements; i++) {
    server_elements[i] = absl::StrCat("Element ", 2 * i);
  }

  // Run Server setup.
  PSI_ASSERT_OK_AND_ASSIGN(
      auto server_setup,
      server_->CreateSetupMessage(fpr, num_client_elements, server_elements));

  // Create Client request.
  PSI_ASSERT_OK_AND_ASSIGN(auto client_request,
                           client->CreateRequest(client_elements));

  // Create Server response.
  PSI_ASSERT_OK_AND_ASSIGN(auto server_response,
                           server_->ProcessRequest(client_request));

  // Compute intersection size.
  PSI_ASSERT_OK_AND_ASSIGN(
      int intersection_size,
      client->GetIntersectionSize(server_setup, server_response));

  // Test if size is approximately as expected (up to 10%).
  EXPECT_GE(intersection_size, num_client_elements / 2);
  EXPECT_LT(intersection_size, (num_client_elements / 2) * 1.1);
}

TEST_F(PsiServerTest, TestArrayIsSortedWhenNotRevealingIntersection) {
  SetUp(false);
  PSI_ASSERT_OK_AND_ASSIGN(auto client, PsiClient::Create(false));
  int num_client_elements = 1000;
  std::vector<std::string> client_elements(num_client_elements);
  for (int i = 0; i < num_client_elements; i++) {
    client_elements[i] = absl::StrCat("Element ", i);
  }
  // Create Client request.
  PSI_ASSERT_OK_AND_ASSIGN(auto client_request,
                           client->CreateRequest(client_elements));
  // Create Server response.
  PSI_ASSERT_OK_AND_ASSIGN(auto server_response,
                           server_->ProcessRequest(client_request));
  rapidjson::Document response;
  ASSERT_FALSE(response.Parse(server_response.data(), server_response.size())
                   .HasParseError());
  ASSERT_TRUE(response.IsObject() && response.HasMember("encrypted_elements"));
  const auto response_array = response["encrypted_elements"].GetArray();
  std::vector<std::string> response_vector(response_array.Size());
  for (int i = 0; i < num_client_elements; i++) {
    response_vector[i] = std::string(response_array[i].GetString(),
                                     response_array[i].GetStringLength());
  }
  EXPECT_TRUE(std::is_sorted(response_vector.begin(), response_vector.end()));
}

TEST_F(PsiServerTest, TestCreatingFromKey) {
  SetUp(false);
  // Get the original private key
  const std::string key_bytes = server_->GetPrivateKeyBytes();

  // Test if the key size is 32 bytes.
  EXPECT_EQ(key_bytes.length(), 32);

  // Create a new server instance from the original key
  PSI_ASSERT_OK_AND_ASSIGN(auto server,
                           PsiServer::CreateFromKey(key_bytes, false));

  int num_client_elements = 100, num_server_elements = 1000;
  double fpr = 0.01;
  std::vector<std::string> client_elements(num_client_elements);
  std::vector<std::string> server_elements(num_server_elements);

  // Create elements to sign
  for (int i = 0; i < num_client_elements; i++) {
    client_elements[i] = absl::StrCat("Element ", i);
  }
  for (int i = 0; i < num_server_elements; i++) {
    server_elements[i] = absl::StrCat("Element ", 2 * i);
  }

  // Run Server setup.
  PSI_ASSERT_OK_AND_ASSIGN(
      auto server_setup,
      server_->CreateSetupMessage(fpr, num_client_elements, server_elements));

  PSI_ASSERT_OK_AND_ASSIGN(
      auto server_setup1,
      server->CreateSetupMessage(fpr, num_client_elements, server_elements));

  // Both setup messages should be the same
  EXPECT_EQ(server_setup, server_setup1);

  // Create a 31-byte key that should be equivalent to a 32-byte null-inserted
  // key.
  const std::string key_bytes2("bcdefghijklmnopqrstuvwxyz123456", 31);
  PSI_ASSERT_OK_AND_ASSIGN(auto server2,
                           PsiServer::CreateFromKey(key_bytes2, false));
  const std::string key_bytes3("\0bcdefghijklmnopqrstuvwxyz123456", 32);
  PSI_ASSERT_OK_AND_ASSIGN(auto server3,
                           PsiServer::CreateFromKey(key_bytes3, false));

  // Run Server setup.
  PSI_ASSERT_OK_AND_ASSIGN(
      auto server_setup2,
      server2->CreateSetupMessage(fpr, num_client_elements, server_elements));

  PSI_ASSERT_OK_AND_ASSIGN(
      auto server_setup3,
      server3->CreateSetupMessage(fpr, num_client_elements, server_elements));
  EXPECT_EQ(server_setup2, server_setup3);
}

TEST_F(PsiServerTest, FailIfRevealIntersectionDoesntMatch) {
  std::string client_request_false =
      R"({"reveal_intersection": false, "encrypted_elements": ["AiHdmxkmF/iOM0fFhny9917QYGcb9jq0GM9mP4L74ecM"]})";
  SetUp(true);
  EXPECT_THAT(server_->ProcessRequest(client_request_false),
              StatusIs(private_join_and_compute::kInvalidArgument,
                       "Client expects `reveal_intersection` = 0, but it is "
                       "actually 1"));
  std::string client_request_true =
      R"({"reveal_intersection": true, "encrypted_elements": ["AiHdmxkmF/iOM0fFhny9917QYGcb9jq0GM9mP4L74ecM"]})";
  SetUp(false);
  EXPECT_THAT(server_->ProcessRequest(client_request_true),
              StatusIs(private_join_and_compute::kInvalidArgument,
                       "Client expects `reveal_intersection` = 1, but it is "
                       "actually 0"));
}

}  // namespace
}  // namespace private_set_intersection