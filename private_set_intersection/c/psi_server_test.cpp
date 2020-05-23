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

#include "private_set_intersection/c/psi_server.h"

#include "absl/strings/str_cat.h"
#include "crypto/ec_commutative_cipher.h"
#include "gtest/gtest.h"
#include "private_set_intersection/c/psi_client.h"
#include "util/status_matchers.h"

namespace private_set_intersection {
namespace {

class PsiServerTest : public ::testing::Test {
 protected:
  void SetUp() {
    char *err;
    int ret = psi_server_create_with_new_key(&server_, &err);
    ASSERT_TRUE(server_ != nullptr);
    ASSERT_TRUE(ret == 0);
  }
  void TearDown() {
    psi_server_delete(&server_);
    ASSERT_TRUE(server_ == nullptr);
  }

  psi_server_ctx server_;
};

TEST_F(PsiServerTest, TestCorrectness) {
  // We use an actual client instance here, since we already test the client
  // on its own in psi_cardinality_client_test.cpp.
  psi_client_ctx client_;
  char *err;
  psi_client_create(&client_, &err);

  ASSERT_TRUE(client_ != nullptr);

  int num_client_elements = 1000, num_server_elements = 10000;
  double fpr = 0.01;
  std::vector<std::string> client_orig_elements(num_client_elements);
  std::vector<psi_client_buffer_t> client_elements(num_client_elements);
  std::vector<std::string> server_orig_elements(num_server_elements);
  std::vector<psi_server_buffer_t> server_elements(num_server_elements);

  // Create elements, such that 50% of the client's elements overlap with the
  // server.
  for (int i = 0; i < num_client_elements; i++) {
    client_orig_elements[i] = absl::StrCat("Element ", i);
    client_elements[i] = {client_orig_elements[i].c_str(),
                          client_orig_elements[i].size()};
  }
  for (int i = 0; i < num_server_elements; i++) {
    server_orig_elements[i] = absl::StrCat("Element ", 2 * i);
    server_elements[i] = {server_orig_elements[i].c_str(),
                          server_orig_elements[i].size()};
  }

  // Run Server setup.
  char *server_setup = nullptr;
  size_t server_setup_buff_len = 0;
  psi_server_create_setup_message(
      server_, fpr, num_client_elements, server_elements.data(),
      server_elements.size(), &server_setup, &server_setup_buff_len, &err);

  ASSERT_TRUE(server_setup != nullptr);
  ASSERT_TRUE(server_setup_buff_len != 0);

  // Create Client request.
  char *client_request = {0};
  size_t req_len = 0;
  int ret = psi_client_create_request(client_, client_elements.data(),
                                      client_elements.size(), &client_request,
                                      &req_len, &err);

  ASSERT_TRUE(ret == 0);
  ASSERT_TRUE(req_len > 0);
  ASSERT_TRUE(client_request != nullptr);

  // Create Server response.
  char *server_response = nullptr;
  size_t response_len = 0;
  ret = psi_server_process_request(server_, {client_request, req_len},
                                   &server_response, &response_len, &err);
  ASSERT_TRUE(server_response != nullptr);
  ASSERT_TRUE(response_len >= 0);
  ASSERT_TRUE(ret == 0);

  // Compute intersection size.
  int64_t intersection_size = 0;
  psi_client_process_response(client_, server_setup, server_response,
                              &intersection_size, &err);

  // Test if size is approximately as expected (up to 10%).
  EXPECT_GE(intersection_size, num_client_elements / 2);
  EXPECT_LT(intersection_size, (num_client_elements / 2) * 1.1);

  psi_server_delete_buffer(server_, &server_setup);
  psi_server_delete_buffer(server_, &server_response);
  psi_client_delete_buffer(client_, &client_request);
}

}  // namespace
}  // namespace private_set_intersection
