#include "psi_cardinality_client_c.h"

#include "internal_utils.h"
#include "psi_cardinality/cpp/psi_cardinality_client.h"

using Client = psi_cardinality::PSICardinalityClient;

int psi_cardinality_client_create(psi_cardinality_client_ctx *ctx,
                                  char **error_out) {
  auto result = Client::Create();
  if (result.ok()) {
    *ctx = std::move(result).ValueOrDie().release();
    return 0;
  }

  return psi_cardinality::c_bindings_internal::generate_error(result.status(),
                                                              error_out);
}

void psi_cardinality_client_delete(psi_cardinality_client_ctx *ctx) {
  auto client = static_cast<Client *>(*ctx);
  if (client == nullptr) {
    return;
  }
  delete client;
  *ctx = nullptr;
}

int psi_cardinality_client_create_request(psi_cardinality_client_ctx ctx,
                                          client_buffer_t *inputs,
                                          uint64_t input_len, char **out,
                                          uint64_t *out_len, char **error_out) {
  auto client = static_cast<Client *>(ctx);
  if (client == nullptr) {
    return psi_cardinality::c_bindings_internal::generate_error(
        ::private_join_and_compute::InvalidArgumentError(
            "invalid client context"),
        error_out);
  }
  std::vector<std::string> in;
  for (size_t idx = 0; idx < input_len; ++idx) {
    in.push_back(std::string(inputs[idx].buff, inputs[idx].buff_len));
  }

  auto result = client->CreateRequest(in);
  if (!result.ok()) {
    return psi_cardinality::c_bindings_internal::generate_error(result.status(),
                                                                error_out);
  }
  auto value = std::move(result).ValueOrDie();

  size_t len = value.size() + 1;
  *out = new char[len];
  strncpy(*out, value.c_str(), len);
  *out_len = uint64_t(len);

  return 0;
}

void psi_cardinality_client_delete_buffer(psi_cardinality_client_ctx ctx,
                                          char **request) {
  delete[] * request;
  *request = nullptr;
}

int psi_cardinality_client_process_response(psi_cardinality_client_ctx ctx,
                                            const char *server_setup,
                                            const char *server_response,
                                            int64_t *out, char **error_out) {
  auto client = static_cast<Client *>(ctx);
  if (client == nullptr) {
    return psi_cardinality::c_bindings_internal::generate_error(
        ::private_join_and_compute::InvalidArgumentError(
            "invalid client context"),
        error_out);
  }
  auto result = client->ProcessResponse(server_setup, server_response);
  if (!result.ok()) {
    return psi_cardinality::c_bindings_internal::generate_error(result.status(),
                                                                error_out);
  }
  if (out != nullptr) {
    *out = result.ValueOrDie();
  }
  return 0;
}
