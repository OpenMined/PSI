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

#include "absl/strings/str_cat.h"
#include "crypto/ec_commutative_cipher.h"
#include "gtest/gtest.h"
#include "private_set_intersection/cpp/psi_client.h"
#include "util/status_matchers.h"

namespace private_set_intersection {
namespace {

class PsiServerTest : public ::testing::Test {
 protected:
  void SetUp() {
    PSI_ASSERT_OK_AND_ASSIGN(server_, PsiServer::CreateWithNewKey());
  }

  std::unique_ptr<PsiServer> server_;
};

TEST_F(PsiServerTest, TestCorrectness) {
  // We use an actual client instance here, since we already test the client
  // on its own in psi_cardinality_client_test.cpp.
  PSI_ASSERT_OK_AND_ASSIGN(auto client, PsiClient::Create());
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
      client->ProcessResponse(server_setup, server_response));

  // Test if size is approximately as expected (up to 10%).
  EXPECT_GE(intersection_size, num_client_elements / 2);
  EXPECT_LT(intersection_size, (num_client_elements / 2) * 1.1);
}

TEST_F(PsiServerTest, TestCreatingFromKey) {
  // Get the original private key
  const std::string key_bytes = server_->GetPrivateKeyBytes();

  // Test if the key size is 32 bytes.
  EXPECT_EQ(key_bytes.length(), 32);

  // Create a new server instance from the original key
  PSI_ASSERT_OK_AND_ASSIGN(auto server, PsiServer::CreateFromKey(key_bytes));

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
  // key
  const std::string key_bytes2("bcdefghijklmnopqrstuvwxyz123456", 31);
  PSI_ASSERT_OK_AND_ASSIGN(auto server2, PsiServer::CreateFromKey(key_bytes2));
  const std::string key_bytes3("\0bcdefghijklmnopqrstuvwxyz123456", 32);
  PSI_ASSERT_OK_AND_ASSIGN(auto server3, PsiServer::CreateFromKey(key_bytes3));

  // Run Server setup.
  PSI_ASSERT_OK_AND_ASSIGN(
      auto server_setup2,
      server2->CreateSetupMessage(fpr, num_client_elements, server_elements));

  PSI_ASSERT_OK_AND_ASSIGN(
      auto server_setup3,
      server3->CreateSetupMessage(fpr, num_client_elements, server_elements));
  EXPECT_EQ(server_setup2, server_setup3);
}

}  // namespace
}  // namespace private_set_intersection