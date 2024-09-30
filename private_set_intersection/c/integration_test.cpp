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

#include <math.h>

#include "absl/container/flat_hash_set.h"
#include "absl/strings/str_cat.h"
#include "gtest/gtest.h"
#include "private_join_and_compute/crypto/ec_commutative_cipher.h"
#include "private_set_intersection/c/package.h"
#include "private_set_intersection/c/psi_client.h"
#include "private_set_intersection/c/psi_server.h"
#include "private_set_intersection/cpp/datastructure/datastructure.h"
#include "util/status_matchers.h"

namespace private_set_intersection {

namespace {

static const char client_key_buff[32] = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
psi_client_buffer_t client_key{*(&client_key_buff), sizeof(client_key_buff)};

static const char server_key_buff[32] = {
    1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16,
    17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
psi_server_buffer_t server_key{*(&server_key_buff), sizeof(server_key_buff)};

const int num_client_inputs = 10, num_server_inputs = 100;
const double fpr = 0.01;
std::vector<std::string> client_orig_elements(num_client_inputs);
std::vector<psi_client_buffer_t> client_inputs(num_client_inputs);
std::vector<std::string> server_orig_elements(num_server_inputs);
std::vector<psi_server_buffer_t> server_inputs(num_server_inputs);

TEST(PackageTest, testVersionFormat) {
  // The regex represents Semantic Versioning syntax (www.semver.org),
  // i.e. three dot-separated numbers, with an optional suffix
  // that starts with a hyphen, to cover alpha/beta releases and
  // release candiates, for example:
  //   1.2.3
  //   1.2.3-beta
  //   1.2.3-RC1
  std::string version_regex = "[0-9]+[.][0-9]+[.][0-9]+(-[A-Za-z0-9]+)?";
#ifdef GTEST_USES_POSIX_RE
  EXPECT_THAT(psi_version(), testing::MatchesRegex(version_regex));
#else
  EXPECT_TRUE(std::regex_match(psi_version(), std::regex(version_regex)));
#endif
}

class Correctness
    : public ::testing::TestWithParam<std::tuple<bool, DataStructure>> {
 protected:
  void SetUp() {
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
  }
  void TearDown() {}
};

TEST_F(Correctness, TestCreatingFromKey) {
  psi_client_ctx client2;
  psi_server_ctx server2;
  char *err;
  int ok = 99;
  ok = psi_client_create_from_key(client_key, false, &client2, &err);
  ASSERT_TRUE(client2 != nullptr);
  ASSERT_TRUE(ok == 0);

  ok = psi_server_create_from_key(server_key, false, &server2, &err);
  ASSERT_TRUE(server2 != nullptr);
  ASSERT_TRUE(ok == 0);

  char *client_key_buff2 = {0};
  size_t key_len = 0;

  ok = psi_client_get_private_key_bytes(client2, &client_key_buff2, &key_len,
                                        &err);
  ASSERT_TRUE(client_key_buff2 != nullptr);
  ASSERT_TRUE(ok == 0);
  ASSERT_TRUE(strncmp(client_key.buff, client_key_buff2, client_key.buff_len) ==
              0);
  EXPECT_EQ(key_len, 32);
  free(client_key_buff2);

  char *server_key_buff2 = {0};
  key_len = 0;

  ok = psi_server_get_private_key_bytes(server2, &server_key_buff2, &key_len,
                                        &err);
  ASSERT_TRUE(server_key_buff != nullptr);
  ASSERT_TRUE(ok == 0);
  ASSERT_TRUE(strncmp(server_key.buff, server_key_buff2, server_key.buff_len) ==
              0);
  EXPECT_EQ(key_len, 32);
  free(server_key_buff2);
}

INSTANTIATE_TEST_CASE_P(
    Integration, Correctness,
    testing::Combine(testing::Values(true, false),
                     testing::Values(DataStructure::Raw, DataStructure::Gcs,
                                     DataStructure::BloomFilter)),
    [](const testing::TestParamInfo<Correctness::ParamType> &info) {
      bool reveal_intersection = std::get<0>(info.param);
      DataStructure ds = std::get<1>(info.param);
      std::string ds_name;
      switch (ds) {
        case DataStructure::Raw:
          ds_name = "raw";
          break;
        case DataStructure::Gcs:
          ds_name = "gcs";
          break;
        case DataStructure::BloomFilter:
          ds_name = "bloomfilter";
          break;
        default: {
          throw std::logic_error("Bad enum variant");
        }
      }

      return absl::StrCat(reveal_intersection ? "true_" : "false_", ds_name);
    });

TEST_P(Correctness, intersection) {
  bool reveal_intersection = std::get<0>(GetParam());
  DataStructure ds = std::get<1>(GetParam());

  psi_client_ctx client;
  psi_server_ctx server;
  char *err;

  int ok = psi_client_create_from_key(client_key, reveal_intersection, &client,
                                      &err);
  ASSERT_TRUE(client != nullptr);
  ASSERT_TRUE(ok == 0);

  ok = psi_server_create_from_key(server_key, reveal_intersection, &server,
                                  &err);
  ASSERT_TRUE(server != nullptr);
  ASSERT_TRUE(ok == 0);

  // Run Server setup.
  char *server_setup = nullptr;
  size_t server_setup_buff_len = 0;
  psi_server_create_setup_message(
      server, fpr, num_client_inputs, server_inputs.data(),
      server_inputs.size(), &server_setup, &server_setup_buff_len, &err, ds);

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
    size_t intersect_len;
    psi_client_get_intersection(client, {server_setup, server_setup_buff_len},
                                {server_response, response_len}, &intersection,
                                &intersect_len, &err);

    absl::flat_hash_set<int64_t> intersection_set(intersection,
                                                  intersection + intersect_len);

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
    EXPECT_LE((double)intersection_size,
              ceil((double(num_client_inputs) / 2.0) * 1.1));
  }
  free(server_setup);
  free(client_request);
  free(server_response);

  psi_server_delete(&server);
  ASSERT_TRUE(server == nullptr);
  psi_client_delete(&client);
  ASSERT_TRUE(client == nullptr);
}

}  // namespace
}  // namespace private_set_intersection
