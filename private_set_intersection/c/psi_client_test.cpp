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

#include "private_set_intersection/c/psi_client.h"

#include "absl/container/flat_hash_set.h"
#include "absl/strings/escaping.h"
#include "absl/strings/str_cat.h"
#include "crypto/ec_commutative_cipher.h"
#include "gtest/gtest.h"
#include "private_set_intersection/cpp/bloom_filter.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "util/status_matchers.h"

namespace private_set_intersection {
namespace {

class PsiClientTest : public ::testing::Test {
 protected:
  void SetUp() {}
  void TearDown() {}
};

void test_corectness(bool reveal_intersection) {
  psi_client_ctx client;

  char *err;
  int ret = psi_client_create_with_new_key(reveal_intersection, &client, &err);
  ASSERT_TRUE(client != nullptr);
  ASSERT_TRUE(ret == 0);
  constexpr int num_client_elements = 1000, num_server_elements = 10000;
  double fpr = 0.01;
  std::vector<psi_client_buffer_t> client_elements(num_client_elements);
  std::vector<std::string> client_orig_elements(num_client_elements);
  std::vector<std::string> server_elements(num_server_elements);

  // Create elements, such that 50% of the client's elements overlap with the
  // server.
  for (int i = 0; i < num_client_elements; i++) {
    client_orig_elements[i] = absl::StrCat("Element ", i);
    client_elements[i] = {client_orig_elements[i].c_str(),
                          client_orig_elements[i].size()};
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
  char *client_request = {0};
  size_t req_len = 0;
  ret = psi_client_create_request(client, client_elements.data(),
                                  client_elements.size(), &client_request,
                                  &req_len, &err);

  ASSERT_TRUE(ret == 0);
  ASSERT_TRUE(req_len > 0);
  ASSERT_TRUE(client_request != nullptr);

  // Unpack client message and re-encrypt.
  rapidjson::Document request, response;
  request.Parse(client_request, strlen(client_request));
  ASSERT_FALSE(request.HasParseError());
  ASSERT_TRUE(request.IsObject());
  const rapidjson::Value &encrypted_elements = request["encrypted_elements"];
  ASSERT_TRUE(encrypted_elements.IsArray());
  response.SetObject();
  rapidjson::Value response_elements;
  response_elements.SetArray();
  std::vector<std::string> reencrypted_elements(num_client_elements);
  for (int i = 0; i < num_client_elements; i++) {
    ASSERT_TRUE(encrypted_elements[i].IsString());
    std::string base64_element(encrypted_elements[i].GetString(),
                               encrypted_elements[i].GetStringLength());
    std::string encrypted_element;
    ASSERT_TRUE(absl::Base64Unescape(base64_element, &encrypted_element));
    PSI_ASSERT_OK_AND_ASSIGN(reencrypted_elements[i],
                             server_ec_cipher->ReEncrypt(encrypted_element));

    base64_element = absl::Base64Escape(reencrypted_elements[i]);
    response_elements.PushBack(rapidjson::Value().SetString(
                                   base64_element.data(), base64_element.size(),
                                   response.GetAllocator()),
                               response.GetAllocator());
  }
  response.AddMember("encrypted_elements", response_elements.Move(),
                     response.GetAllocator());
  response.AddMember("reveal_intersection",
                     rapidjson::Value(reveal_intersection).Move(),
                     response.GetAllocator());

  // Encode re-encrypted messages as JSON.
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<decltype(buffer)> writer(buffer);
  response.Accept(writer);
  std::string server_response(buffer.GetString());

  // Compute intersection.
  if (reveal_intersection) {
    int64_t *intersection;
    size_t intersectlen;
    psi_client_get_intersection(client, server_setup.c_str(),
                                server_response.c_str(), &intersection,
                                &intersectlen, &err);

    absl::flat_hash_set<int64_t> intersection_set(intersection,
                                                  intersection + intersectlen);

    // Test if all even elements are present.
    for (int i = 0; i < num_client_elements; i++) {
      if (i % 2) {
        EXPECT_FALSE(intersection_set.contains(i));
      } else {
        EXPECT_TRUE(intersection_set.contains(i));
      }
    }
  } else {
    int64_t intersection_size = 0;
    ret = psi_client_get_intersection_size(client, server_setup.c_str(),
                                           server_response.c_str(),
                                           &intersection_size, &err);
    ASSERT_TRUE(ret == 0);
    ASSERT_TRUE(intersection_size > 0);
    // Test if size is approximately as expected (up to 10%).

    EXPECT_GE(intersection_size, num_client_elements / 2);
    EXPECT_LT(intersection_size, (num_client_elements / 2) * 1.1);
  }
  free(client_request);

  psi_client_delete(&client);

  ASSERT_TRUE(client == nullptr);
}
TEST_F(PsiClientTest, TestCorrectnessSize) { test_corectness(false); }

TEST_F(PsiClientTest, TestCorrectness) { test_corectness(true); }

}  // namespace
}  // namespace private_set_intersection
