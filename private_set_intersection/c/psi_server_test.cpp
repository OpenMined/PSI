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

#include "absl/container/flat_hash_set.h"
#include "absl/strings/str_cat.h"
#include "gtest/gtest.h"
#include "private_join_and_compute/crypto/ec_commutative_cipher.h"
#include "private_set_intersection/c/psi_client.h"
#include "private_set_intersection/cpp/datastructure/datastructure.h"
#include "util/status_matchers.h"

namespace private_set_intersection {
namespace {

const int num_client_inputs = 10, num_server_inputs = 100;
const double fpr = 0.01;

class PsiServerTest : public ::testing::Test {
 protected:
  void SetUp() {}
  void TearDown() {}
};

void test_correctness(bool reveal_intersection) {
  psi_server_ctx server;
  char *err;

  int ok = psi_server_create_with_new_key(reveal_intersection, &server, &err);
  ASSERT_TRUE(server != nullptr);
  ASSERT_TRUE(ok == 0);

  psi_client_ctx client;
  ok = psi_client_create_with_new_key(reveal_intersection, &client, &err);
  ASSERT_TRUE(client != nullptr);
  ASSERT_TRUE(ok == 0);

  std::vector<std::string> client_orig_elements(num_client_inputs);
  std::vector<psi_client_buffer_t> client_inputs(num_client_inputs);
  std::vector<std::string> server_orig_elements(num_server_inputs);
  std::vector<psi_server_buffer_t> server_inputs(num_server_inputs);

  // Create elements, such that 50% of the client's elements overlap with the
  // server.
  for (int i = 0; i < num_client_inputs; i++) {
    client_orig_elements[i] = absl::StrCat("Element ", i);
    client_inputs[i] = {client_orig_elements[i].c_str(),
                        client_orig_elements[i].size()};
  }
  for (int i = 0; i < num_server_inputs; i++) {
    server_orig_elements[i] = absl::StrCat("Element ", i * 2);
    server_inputs[i] = {server_orig_elements[i].c_str(),
                        server_orig_elements[i].size()};
  }

  // Run Server setup.
  char *server_setup = nullptr;
  size_t server_setup_buff_len = 0;
  psi_server_create_setup_message(server, fpr, num_client_inputs,
                                  server_inputs.data(), server_inputs.size(),
                                  &server_setup, &server_setup_buff_len, &err,
                                  DataStructure::Gcs);

  ASSERT_TRUE(server_setup != nullptr);
  ASSERT_TRUE(server_setup_buff_len != 0);

  // Create Client request.
  char *client_request = {0};
  size_t req_len = 0;
  ok = psi_client_create_request(client, client_inputs.data(),
                                 client_inputs.size(), &client_request,
                                 &req_len, &err);

  ASSERT_TRUE(ok == 0);
  ASSERT_TRUE(req_len > 0);
  ASSERT_TRUE(client_request != nullptr);

  // Create Server response.
  char *server_response = nullptr;
  size_t response_len = 0;
  ok = psi_server_process_request(server, {client_request, req_len},
                                  &server_response, &response_len, &err);
  ASSERT_TRUE(server_response != nullptr);
  ASSERT_TRUE(response_len >= 0);
  ASSERT_TRUE(ok == 0);

  if (reveal_intersection) {
    // Compute intersection.
    int64_t *intersection;
    size_t intersectlen;
    psi_client_get_intersection(client, {server_setup, server_setup_buff_len},
                                {server_response, response_len}, &intersection,
                                &intersectlen, &err);

    absl::flat_hash_set<int64_t> intersection_set(intersection,
                                                  intersection + intersectlen);

    // Test if all even elements are present.
    for (int i = 0; i < num_client_inputs; i++) {
      if (i % 2) {
        EXPECT_FALSE(intersection_set.contains(i));
      } else {
        EXPECT_TRUE(intersection_set.contains(i));
      }
    }

  } else {
    // Compute intersection size.
    int64_t intersection_size = 0;
    psi_client_get_intersection_size(
        client, {server_setup, server_setup_buff_len},
        {server_response, response_len}, &intersection_size, &err);

    // Test if size is approximately as expected (up to 10%).
    EXPECT_GE(intersection_size, num_client_inputs / 2);
    EXPECT_LT(intersection_size, (num_client_inputs / 2) * 1.1);
  }
  free(server_setup);
  free(server_response);
  free(client_request);

  psi_server_delete(&server);
  ASSERT_TRUE(server == nullptr);
  psi_client_delete(&client);
  ASSERT_TRUE(client == nullptr);
}

TEST_F(PsiServerTest, TestCorrectnessSize) { test_correctness(false); }

TEST_F(PsiServerTest, TestCorrectnessIntersection) { test_correctness(true); }
}  // namespace
}  // namespace private_set_intersection
