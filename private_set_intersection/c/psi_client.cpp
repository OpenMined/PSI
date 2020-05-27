#include "private_set_intersection/c/psi_client.h"

#include "private_set_intersection/c/internal_utils.h"
#include "private_set_intersection/cpp/psi_client.h"

using Client = private_set_intersection::PsiClient;

int psi_client_create(psi_client_ctx *ctx, bool reveal_intersection, char **error_out) {
  auto result = Client::Create(reveal_intersection);
  if (result.ok()) {
    *ctx = std::move(result).ValueOrDie().release();
    return 0;
  }

  return private_set_intersection::c_bindings_internal::generate_error(
      result.status(), error_out);
}

void psi_client_delete(psi_client_ctx *ctx) {
  auto client = static_cast<Client *>(*ctx);
  if (client == nullptr) {
    return;
  }
  delete client;
  *ctx = nullptr;
}

int psi_client_create_request(psi_client_ctx ctx, psi_client_buffer_t *inputs,
                              size_t input_len, char **output, size_t *out_len,
                              char **error_out) {
  auto client = static_cast<Client *>(ctx);
  if (client == nullptr) {
    return private_set_intersection::c_bindings_internal::generate_error(
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
    return private_set_intersection::c_bindings_internal::generate_error(
        result.status(), error_out);
  }
  auto value = std::move(result).ValueOrDie();

  size_t len = value.size() + 1;
  *output = new char[len];
  strncpy(*output, value.c_str(), len);
  *out_len = len;

  return 0;
}

void psi_client_delete_buffer(psi_client_ctx ctx, char **request) {
  delete[] * request;
  *request = nullptr;
}

int psi_client_get_intersection_size(psi_client_ctx ctx, const char *server_setup,
                                const char *server_response, int64_t *out,
                                char **error_out) {
  auto client = static_cast<Client *>(ctx);
  if (client == nullptr) {
    return private_set_intersection::c_bindings_internal::generate_error(
        ::private_join_and_compute::InvalidArgumentError(
            "invalid client context"),
        error_out);
  }
  auto result = client->GetIntersectionSize(server_setup, server_response);
  if (!result.ok()) {
    return private_set_intersection::c_bindings_internal::generate_error(
        result.status(), error_out);
  }
  if (out != nullptr) {
    *out = result.ValueOrDie();
  }
  return 0;
}

int psi_client_get_intersection(psi_client_ctx ctx, const char *server_setup,
                                const char *server_response, int64_t *out,
                                char **error_out) {
  auto client = static_cast<Client *>(ctx);
  if (client == nullptr) {
    return private_set_intersection::c_bindings_internal::generate_error(
        ::private_join_and_compute::InvalidArgumentError(
            "invalid client context"),
        error_out);
  }
  auto result = client->GetIntersection(server_setup, server_response);
  if (!result.ok()) {
    return private_set_intersection::c_bindings_internal::generate_error(
        result.status(), error_out);
  }
  if (out != nullptr) {
    *out = result.ValueOrDie()[0];
  }
  return 0;
}
