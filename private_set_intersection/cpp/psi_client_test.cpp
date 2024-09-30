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

#include "private_set_intersection/cpp/psi_client.h"

#include <math.h>

#include "absl/container/flat_hash_set.h"
#include "absl/strings/escaping.h"
#include "absl/strings/str_cat.h"
#include "gtest/gtest.h"
#include "private_join_and_compute/crypto/ec_commutative_cipher.h"
#include "private_set_intersection/cpp/datastructure/gcs.h"
#include "util/status_matchers.h"

namespace private_set_intersection {
namespace {

class PsiClientTest : public ::testing::Test {
 protected:
  void SetUp(bool reveal_intersection) {
    PSI_ASSERT_OK_AND_ASSIGN(client_,
                             PsiClient::CreateWithNewKey(reveal_intersection));
    PSI_ASSERT_OK_AND_ASSIGN(
        server_ec_cipher_,
        ::private_join_and_compute::ECCommutativeCipher::CreateWithNewKey(
            NID_X9_62_prime256v1,
            ::private_join_and_compute::ECCommutativeCipher::HashType::SHA256));
  }

  void CreateDummySetupMessage(absl::Span<const std::string> server_elements,
                               double fpr,
                               psi_proto::ServerSetup* server_setup) {
    auto num_server_elements = static_cast<int64_t>(server_elements.size());
    std::vector<std::string> elements;
    elements.reserve(num_server_elements);

    for (int i = 0; i < num_server_elements; i++) {
      PSI_ASSERT_OK_AND_ASSIGN(std::string encrypted_element,
                               server_ec_cipher_->Encrypt(server_elements[i]));
      elements.push_back(encrypted_element);
    }

    // Insert server elements into GCS.
    PSI_ASSERT_OK_AND_ASSIGN(
        auto gcs,
        GCS::Create(fpr, (int64_t)elements.size(),
                    absl::MakeConstSpan(&elements[0], elements.size())));
    *server_setup = gcs->ToProtobuf();
  }

  void CreateDummyResponse(const psi_proto::Request& client_request,
                           psi_proto::Response* server_response) {
    ASSERT_TRUE(client_request.IsInitialized());
    ASSERT_TRUE(server_response->IsInitialized());

    // Clear the elements
    server_response->clear_encrypted_elements();

    const auto& encrypted_elements = client_request.encrypted_elements();
    const std::int64_t num_request_elements =
        static_cast<std::int64_t>(encrypted_elements.size());

    // Re-encrypt the request's elements and add to the response
    for (int i = 0; i < num_request_elements; i++) {
      PSI_ASSERT_OK_AND_ASSIGN(
          std::string encrypted,
          server_ec_cipher_->ReEncrypt(encrypted_elements[i]));
      server_response->add_encrypted_elements(encrypted);
    }
  }

  std::unique_ptr<PsiClient> client_;
  std::unique_ptr<private_join_and_compute::ECCommutativeCipher>
      server_ec_cipher_;
};

TEST_F(PsiClientTest, TestCreatingFromKey) {
  SetUp(false);
  // Get the original private key
  const std::string key_bytes = client_->GetPrivateKeyBytes();

  // Test if the key size is 32 bytes.
  EXPECT_EQ(key_bytes.length(), 32);

  // Create a new client instance from the original key
  PSI_ASSERT_OK_AND_ASSIGN(auto client1,
                           PsiClient::CreateFromKey(key_bytes, false));

  int num_client_elements = 100;
  std::vector<std::string> client_elements(num_client_elements);

  // Create elements to sign
  for (int i = 0; i < num_client_elements; i++) {
    client_elements[i] = absl::StrCat("Element ", i);
  }

  // Run client request.
  PSI_ASSERT_OK_AND_ASSIGN(auto client_request0,
                           client_->CreateRequest(client_elements));
  PSI_ASSERT_OK_AND_ASSIGN(auto client_request1,
                           client1->CreateRequest(client_elements));

  // Both requests should be the same
  EXPECT_EQ(client_request0.reveal_intersection(),
            client_request1.reveal_intersection());
  const auto& elements0 = client_request0.encrypted_elements();
  const auto& elements1 = client_request1.encrypted_elements();
  ASSERT_TRUE(elements0.size() == elements1.size());
  for (int i = 0; i < elements0.size(); i++) {
    EXPECT_EQ(elements0[i], elements1[i]);
  }

  // Create a 31-byte key that should be equivalent to a 32-byte null-inserted
  // key.
  const std::string key_bytes2("bcdefghijklmnopqrstuvwxyz123456", 31);
  PSI_ASSERT_OK_AND_ASSIGN(auto client2,
                           PsiClient::CreateFromKey(key_bytes2, false));
  const std::string key_bytes3("\0bcdefghijklmnopqrstuvwxyz123456", 32);
  PSI_ASSERT_OK_AND_ASSIGN(auto client3,
                           PsiClient::CreateFromKey(key_bytes3, false));

  // Run client request.
  PSI_ASSERT_OK_AND_ASSIGN(auto client_request2,
                           client2->CreateRequest(client_elements));
  PSI_ASSERT_OK_AND_ASSIGN(auto client_request3,
                           client3->CreateRequest(client_elements));
  EXPECT_EQ(client_request2.reveal_intersection(),
            client_request3.reveal_intersection());
  const auto& elements2 = client_request0.encrypted_elements();
  const auto& elements3 = client_request1.encrypted_elements();
  ASSERT_TRUE(elements2.size() == elements3.size());
  for (int i = 0; i < elements2.size(); i++) {
    EXPECT_EQ(elements2[i], elements3[i]);
  }
}

TEST_F(PsiClientTest, TestCorrectnessIntersection) {
  SetUp(true);
  int num_client_elements = 1000, num_server_elements = 10000;
  double fpr = 1e-12;
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
  psi_proto::ServerSetup server_setup;
  CreateDummySetupMessage(server_elements, fpr / num_client_elements,
                          &server_setup);

  // Compute client request.
  PSI_ASSERT_OK_AND_ASSIGN(psi_proto::Request client_request,
                           client_->CreateRequest(client_elements));

  psi_proto::Response server_response;
  CreateDummyResponse(client_request, &server_response);

  // Compute intersection.
  PSI_ASSERT_OK_AND_ASSIGN(
      std::vector<int64_t> intersection,
      client_->GetIntersection(server_setup, server_response));
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

TEST_F(PsiClientTest, TestCorrectnessIntersectionSize) {
  SetUp(false);
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
  psi_proto::ServerSetup server_setup;
  CreateDummySetupMessage(server_elements, fpr / num_client_elements,
                          &server_setup);

  // Compute client request.
  PSI_ASSERT_OK_AND_ASSIGN(psi_proto::Request client_request,
                           client_->CreateRequest(client_elements));

  psi_proto::Response server_response;
  CreateDummyResponse(client_request, &server_response);

  // Compute intersection size.
  PSI_ASSERT_OK_AND_ASSIGN(
      int64_t intersection_size,
      client_->GetIntersectionSize(server_setup, server_response));

  // Test if size is approximately as expected (up to 10%).
  EXPECT_GE(intersection_size, num_client_elements / 2);
  EXPECT_LE((double)intersection_size,
            ceil(((double)num_client_elements / 2.0) * 1.1));
}

TEST_F(PsiClientTest, FailIfRevealIntersectionDoesntMatch) {
  SetUp(false);
  psi_proto::ServerSetup server_setup;
  psi_proto::Response response;
  EXPECT_THAT(
      client_->GetIntersection(server_setup, response),
      StatusIs(
          absl::StatusCode::kInvalidArgument,
          "GetIntersection called on PsiClient with reveal_intersection == "
          "false"));
}

}  // namespace
}  // namespace private_set_intersection
