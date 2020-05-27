#include "private_set_intersection/c/psi_server.h"

#include "private_set_intersection/c/internal_utils.h"
#include "private_set_intersection/cpp/psi_server.h"

using Server = private_set_intersection::PsiServer;
int psi_server_create_with_new_key(psi_server_ctx *ctx,
                                   bool reveal_intersection, char **error_out) {
  auto result = Server::CreateWithNewKey(reveal_intersection);
  if (result.ok()) {
    *ctx = std::move(result).ValueOrDie().release();
    return 0;
  }

  return private_set_intersection::c_bindings_internal::generate_error(
      result.status(), error_out);
}

int psi_server_create_from_key(psi_server_buffer_t key_bytes,
                               bool reveal_intersection, psi_server_ctx *ctx,
                               char **error_out) {
  auto result = Server::CreateFromKey(
      std::string(key_bytes.buff, key_bytes.buff_len), reveal_intersection);
  if (result.ok()) {
    *ctx = std::move(result).ValueOrDie().release();
    return 0;
  }
  return private_set_intersection::c_bindings_internal::generate_error(
      result.status(), error_out);
}

void psi_server_delete(psi_server_ctx *ctx) {
  auto server = static_cast<Server *>(*ctx);
  if (server == nullptr) {
    return;
  }
  delete server;
  *ctx = nullptr;
}

int psi_server_create_setup_message(psi_server_ctx ctx, double fpr,
                                    int64_t num_client_inputs,
                                    psi_server_buffer_t *input,
                                    size_t input_len, char **output,
                                    size_t *output_len, char **error_out) {
  auto server = static_cast<Server *>(ctx);
  if (server == nullptr) {
    return private_set_intersection::c_bindings_internal::generate_error(
        ::private_join_and_compute::InvalidArgumentError(
            "invalid server context"),
        error_out);
  }

  std::vector<std::string> in;
  for (size_t idx = 0; idx < input_len; ++idx) {
    in.push_back(std::string(input[idx].buff, input[idx].buff_len));
  }

  auto result = server->CreateSetupMessage(fpr, num_client_inputs, in);
  if (!result.ok()) {
    return private_set_intersection::c_bindings_internal::generate_error(
        result.status(), error_out);
  }

  auto value = result.ValueOrDie();
  size_t len = value.size() + 1;
  *output = (char *)malloc(len * sizeof(char));
  strncpy(*output, value.c_str(), len);
  *output_len = len;

  return 0;
}

int psi_server_process_request(psi_server_ctx ctx,
                               psi_server_buffer_t client_request,
                               char **output, size_t *output_len,
                               char **error_out) {
  auto server = static_cast<Server *>(ctx);
  if (server == nullptr) {
    return private_set_intersection::c_bindings_internal::generate_error(
        ::private_join_and_compute::InvalidArgumentError(
            "invalid server context"),
        error_out);
  }

  auto result = server->ProcessRequest(
      std::string(client_request.buff, client_request.buff_len));
  if (!result.ok()) {
    return private_set_intersection::c_bindings_internal::generate_error(
        result.status(), error_out);
  }

  auto value = result.ValueOrDie();
  size_t len = value.size() + 1;
  *output = (char *)malloc(len * sizeof(char));
  strncpy(*output, value.c_str(), len);
  *output_len = len;

  return 0;
}

int psi_server_get_private_key_bytes(psi_server_ctx ctx, char **output,
                                     size_t *output_len, char **error_out) {
  auto server = static_cast<Server *>(ctx);
  if (server == nullptr) {
    return private_set_intersection::c_bindings_internal::generate_error(
        ::private_join_and_compute::InvalidArgumentError(
            "invalid server context"),
        error_out);
  }
  auto value = server->GetPrivateKeyBytes();
  size_t len = value.size();

  // Private keys are raw bytes -> Use memcpy instead of strncpy.
  *output = (char *)malloc(len * sizeof(char));
  memcpy(*output, value.data(), len);
  *output_len = len;

  return 0;
}
