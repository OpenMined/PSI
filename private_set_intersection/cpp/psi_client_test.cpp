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

#include "absl/strings/escaping.h"
#include "absl/strings/str_cat.h"
#include "private_set_intersection/cpp/bloom_filter.h"
#include "crypto/ec_commutative_cipher.h"
#include "gtest/gtest.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "util/status_matchers.h"

namespace private_set_intersection {
namespace {

class PsiClientTest : public ::testing::Test {
 protected:
  void SetUp() {
    PSI_ASSERT_OK_AND_ASSIGN(client_, PsiClient::Create());
  }

  std::unique_ptr<PsiClient> client_;
};

TEST_F(PsiClientTest, TestCorrectness) {
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

  // Insert server elements into Bloom filter.
  PSI_ASSERT_OK_AND_ASSIGN(
      auto bloom_filter,
      BloomFilter::Create(fpr / num_client_elements, num_server_elements));
  PSI_ASSERT_OK_AND_ASSIGN(
      auto server_ec_cipher,
      ::private_join_and_compute::ECCommutativeCipher::CreateWithNewKey(
          NID_X9_62_prime256v1,
          ::private_join_and_compute::ECCommutativeCipher::HashType::SHA256));
  for (int i = 0; i < num_server_elements; i++) {
    PSI_ASSERT_OK_AND_ASSIGN(std::string encrypted_element,
                             server_ec_cipher->Encrypt(server_elements[i]));
    bloom_filter->Add(encrypted_element);
  }
  std::string server_setup = bloom_filter->ToJSON();

  // Compute client request.
  PSI_ASSERT_OK_AND_ASSIGN(std::string client_request,
                           client_->CreateRequest(client_elements));

  // Unpack client message and re-encrypt.
  rapidjson::Document request, response;
  request.Parse(client_request.data(), client_request.size());
  ASSERT_FALSE(request.HasParseError());
  ASSERT_TRUE(request.IsArray());
  response.SetArray();
  std::vector<std::string> reencrypted_elements(num_client_elements);
  for (int i = 0; i < num_client_elements; i++) {
    ASSERT_TRUE(request[i].IsString());
    std::string base64_element(request[i].GetString(),
                               request[i].GetStringLength());
    std::string encrypted_element;
    ASSERT_TRUE(absl::Base64Unescape(base64_element, &encrypted_element));
    PSI_ASSERT_OK_AND_ASSIGN(reencrypted_elements[i],
                             server_ec_cipher->ReEncrypt(encrypted_element));

    base64_element = absl::Base64Escape(reencrypted_elements[i]);
    response.PushBack(rapidjson::Value().SetString(base64_element.data(),
                                                   base64_element.size(),
                                                   response.GetAllocator()),
                      response.GetAllocator());
  }

  // Encode re-encrypted messages as JSON.
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<decltype(buffer)> writer(buffer);
  response.Accept(writer);
  std::string server_response(buffer.GetString());

  // Compute intersection size.
  PSI_ASSERT_OK_AND_ASSIGN(
      int64_t intersection_size,
      client_->ProcessResponse(server_setup, server_response));

  // Test if size is approximately as expected (up to 10%).
  EXPECT_GE(intersection_size, num_client_elements / 2);
  EXPECT_LT(intersection_size, (num_client_elements / 2) * 1.1);
}

}  // namespace
}  // namespace private_set_intersection