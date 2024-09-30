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

#include <math.h>

#include "absl/container/flat_hash_set.h"
#include "absl/strings/escaping.h"
#include "absl/strings/str_cat.h"
#include "gtest/gtest.h"
#include "private_join_and_compute/crypto/ec_commutative_cipher.h"
#include "private_set_intersection/cpp/psi_client.h"
#include "private_set_intersection/proto/psi.pb.h"
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
  PSI_ASSERT_OK_AND_ASSIGN(auto client, PsiClient::CreateWithNewKey(true));
  int num_client_elements = 1000, num_server_elements = 10000;
  double fpr = 0.0001;
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
  PSI_ASSERT_OK_AND_ASSIGN(
      auto server_setup2,
      server_->CreateSetupMessage(fpr, num_client_elements, server_elements,
                                  DataStructure::BloomFilter));

  // Create Client request.
  PSI_ASSERT_OK_AND_ASSIGN(auto client_request,
                           client->CreateRequest(client_elements));
  PSI_ASSERT_OK_AND_ASSIGN(auto client_request2,
                           client->CreateRequest(client_elements));

  // Create Server response.
  PSI_ASSERT_OK_AND_ASSIGN(auto server_response,
                           server_->ProcessRequest(client_request));
  PSI_ASSERT_OK_AND_ASSIGN(auto server_response2,
                           server_->ProcessRequest(client_request2));

  // Compute intersection.
  PSI_ASSERT_OK_AND_ASSIGN(
      std::vector<int64_t> intersection,
      client->GetIntersection(server_setup, server_response));
  absl::flat_hash_set<int64_t> intersection_set(intersection.begin(),
                                                intersection.end());
  PSI_ASSERT_OK_AND_ASSIGN(
      std::vector<int64_t> intersection2,
      client->GetIntersection(server_setup2, server_response2));
  absl::flat_hash_set<int64_t> intersection_set2(intersection2.begin(),
                                                 intersection2.end());

  // Test if all even elements are present.
  for (int i = 0; i < num_client_elements; i++) {
    if (i % 2) {
      EXPECT_FALSE(intersection_set.contains(i));
      EXPECT_FALSE(intersection_set2.contains(i));
    } else {
      EXPECT_TRUE(intersection_set.contains(i));
      EXPECT_TRUE(intersection_set2.contains(i));
    }
  }
}

TEST_F(PsiServerTest, TestCorrectnessIntersectionSize) {
  SetUp(false);
  // We use an actual client instance here, since we already test the client
  // on its own in psi_client_test.cpp.
  PSI_ASSERT_OK_AND_ASSIGN(auto client, PsiClient::CreateWithNewKey(false));
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
  EXPECT_LE((double)intersection_size,
            ceil(((double)num_client_elements / 2.0) * 1.1));
}

TEST_F(PsiServerTest, TestArrayIsSortedWhenNotRevealingIntersection) {
  SetUp(false);
  PSI_ASSERT_OK_AND_ASSIGN(auto client, PsiClient::CreateWithNewKey(false));
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

  ASSERT_TRUE(server_response.IsInitialized());
  const auto& response_array = server_response.encrypted_elements();
  EXPECT_TRUE(std::is_sorted(response_array.begin(), response_array.end()));
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
  EXPECT_EQ(server_setup.gcs().div(), server_setup1.gcs().div());
  EXPECT_EQ(server_setup.gcs().hash_range(), server_setup1.gcs().hash_range());
  EXPECT_EQ(server_setup.gcs().bits(), server_setup1.gcs().bits());

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
  EXPECT_EQ(server_setup2.gcs().div(), server_setup3.gcs().div());
  EXPECT_EQ(server_setup2.gcs().hash_range(), server_setup3.gcs().hash_range());
  EXPECT_EQ(server_setup2.gcs().bits(), server_setup3.gcs().bits());
}

TEST_F(PsiServerTest, FailIfRevealIntersectionDoesntMatch) {
  psi_proto::Request client_request;

  // Set the reveal intersection flag
  client_request.set_reveal_intersection(false);

  // Set a random encrypted element
  std::string encrypted;
  ASSERT_TRUE(absl::Base64Unescape(
      "AiHdmxkmF/iOM0fFhny9917QYGcb9jq0GM9mP4L74ecM", &encrypted));
  client_request.add_encrypted_elements(encrypted);

  SetUp(true);
  EXPECT_THAT(server_->ProcessRequest(client_request),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Client expects `reveal_intersection` = 0, but it is "
                       "actually 1"));

  // Flip the reveal intersection flag
  client_request.set_reveal_intersection(true);

  SetUp(false);
  EXPECT_THAT(server_->ProcessRequest(client_request),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       "Client expects `reveal_intersection` = 1, but it is "
                       "actually 0"));
}

}  // namespace
}  // namespace private_set_intersection
