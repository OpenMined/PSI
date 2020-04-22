#include "psi_cardinality_server.h"
#include "absl/strings/str_cat.h"
#include "crypto/ec_commutative_cipher.h"
#include "gtest/gtest.h"
#include "psi_cardinality_client.h"
#include "util/status_matchers.h"

namespace psi_cardinality {
namespace {

class PSICardinalityServerTest : public ::testing::Test {
 protected:
  void SetUp() {
    PSI_ASSERT_OK_AND_ASSIGN(server_, PSICardinalityServer::CreateWithNewkey());
  }

  std::unique_ptr<PSICardinalityServer> server_;
};

TEST_F(PSICardinalityServerTest, TestCorrectness) {
  // We use an actual client instance here, since we already test the client on
  // its own in psi_cardinality_client_test.cpp.
  PSI_ASSERT_OK_AND_ASSIGN(auto client, PSICardinalityClient::Create());
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
  PSI_ASSERT_OK_AND_ASSIGN(auto server_setup,
                           server_->CreateSetupMessage(fpr, server_elements));

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

}  // namespace
}  // namespace psi_cardinality