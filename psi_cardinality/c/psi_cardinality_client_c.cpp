#include "psi_cardinality_client_c.h"

#include "internal_utils.h"
#include "psi_cardinality_client.h"

using namespace psi_cardinality;

int psi_cardinality_client_create(psi_cardinality_client_ctx *ctx,
                                  char **error_out) {
  auto result = PSICardinalityClient::Create();
  if (result.ok()) {
    *ctx = std::move(result).ValueOrDie().release();
    return 0;
  }

  return generate_error(result.status(), error_out);
}

void psi_cardinality_client_delete(psi_cardinality_client_ctx *ctx) {
  auto client = static_cast<PSICardinalityClient *>(*ctx);
  if (client == nullptr) {
    return;
  }
  delete client;
  *ctx = nullptr;
}

int psi_cardinality_client_create_request(psi_cardinality_client_ctx ctx,
                                          client_buffer_t *inputs,
                                          size_t input_len, char **out,
                                          size_t *out_len) {
  auto client = static_cast<PSICardinalityClient *>(ctx);
  if (client == nullptr) {
    return -1;
  }
  std::vector<std::string> in;
  for (size_t idx = 0; idx < input_len; ++idx) {
    in.push_back(std::string(inputs[idx].buff, inputs[idx].buff_len));
  }

  auto result = client->CreateRequest(in);
  if (!result.ok()) {
    return -1;
  }
  auto value = std::move(result).ValueOrDie();

  size_t len = value.size() + 1;
  *out = new char[len];
  strncpy(*out, value.c_str(), len);
  *out_len = len;

  return 0;
}

void psi_cardinality_client_delete_buffer(psi_cardinality_client_ctx ctx,
                                          char **request) {
  delete[] * request;
  *request = nullptr;
}

int64_t psi_cardinality_client_process_response(psi_cardinality_client_ctx ctx,
                                                const char *server_setup,
                                                const char *server_response) {
  auto client = static_cast<PSICardinalityClient *>(ctx);
  if (client == nullptr) {
    return -1;
  }
  auto result = client->ProcessResponse(server_setup, server_response);
  if (!result.ok()) {
    return -1;
  }
  return result.ValueOrDie();
}
