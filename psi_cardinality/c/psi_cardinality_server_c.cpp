#include "psi_cardinality_server_c.h"

#include "internal_utils.h"
#include "psi_cardinality/cpp/psi_cardinality_server.h"

using Server = psi_cardinality::PSICardinalityServer;

int psi_cardinality_server_create_with_new_key(psi_cardinality_server_ctx *ctx,
                                               char **error_out) {
  auto result = Server::CreateWithNewKey();
  if (result.ok()) {
    *ctx = std::move(result).ValueOrDie().release();
    return 0;
  }

  return psi_cardinality::c_bindings_internal::generate_error(result.status(),
                                                              error_out);
}

int psi_cardinality_server_create_from_key(server_buffer_t key_bytes,
                                           psi_cardinality_server_ctx *ctx,
                                           char **error_out) {
  auto result =
      Server::CreateFromKey(std::string(key_bytes.buff, key_bytes.buff_len));
  if (result.ok()) {
    *ctx = std::move(result).ValueOrDie().release();
    return 0;
  }
  return psi_cardinality::c_bindings_internal::generate_error(result.status(),
                                                              error_out);
}

void psi_cardinality_server_delete(psi_cardinality_server_ctx *ctx) {
  auto server = static_cast<Server *>(*ctx);
  if (server == nullptr) {
    return;
  }
  delete server;
  *ctx = nullptr;
}

int psi_cardinality_server_create_setup_message(psi_cardinality_server_ctx ctx,
                                                double fpr,
                                                int64_t num_client_inputs,
                                                server_buffer_t *input,
                                                size_t input_len, char **output,
                                                size_t *output_len) {
  auto server = static_cast<Server *>(ctx);
  if (server == nullptr) {
    return -1;
  }

  std::vector<std::string> in;
  for (size_t idx = 0; idx < input_len; ++idx) {
    in.push_back(std::string(input[idx].buff, input[idx].buff_len));
  }

  auto result = server->CreateSetupMessage(fpr, num_client_inputs, in);
  if (!result.ok()) {
    return -1;
  }

  auto value = result.ValueOrDie();
  size_t len = value.size() + 1;
  *output = new char[len];
  strncpy(*output, value.c_str(), len);
  *output_len = len;

  return 0;
}

int psi_cardinality_server_process_request(psi_cardinality_server_ctx ctx,
                                           server_buffer_t client_request,
                                           char **output, size_t *output_len) {
  auto server = static_cast<Server *>(ctx);
  if (server == nullptr) {
    return -1;
  }

  auto result = server->ProcessRequest(
      std::string(client_request.buff, client_request.buff_len));
  if (!result.ok()) {
    return -1;
  }

  auto value = result.ValueOrDie();
  size_t len = value.size() + 1;
  *output = new char[len];
  strncpy(*output, value.c_str(), len);
  *output_len = len;

  return 0;
}

int psi_cardinality_server_get_private_key_bytes(psi_cardinality_server_ctx ctx,
                                                 char **output,
                                                 size_t *output_len) {
  auto server = static_cast<Server *>(ctx);
  if (server == nullptr) {
    return -1;
  }
  auto value = server->GetPrivateKeyBytes();
  size_t len = value.size() + 1;
  *output = new char[len];
  strncpy(*output, value.c_str(), len);
  *output_len = len;

  return 0;
}

void psi_cardinality_server_delete_buffer(psi_cardinality_server_ctx ctx,
                                          char **input) {
  delete[] * input;
  *input = nullptr;
}
