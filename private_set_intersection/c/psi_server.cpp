#include "private_set_intersection/c/psi_server.h"

#include <algorithm>

#include "private_set_intersection/c/internal_utils.h"
#include "private_set_intersection/cpp/datastructure/datastructure.h"
#include "private_set_intersection/cpp/psi_server.h"

namespace {
using private_set_intersection::DataStructure;
using private_set_intersection::PsiServer;
using private_set_intersection::c_bindings_internal::generate_error;
}  // namespace

int psi_server_create_with_new_key(bool reveal_intersection,
                                   psi_server_ctx *ctx, char **error_out) {
  auto result = PsiServer::CreateWithNewKey(reveal_intersection);
  if (result.ok()) {
    *ctx = result->release();
    return 0;
  }

  return generate_error(result.status(), error_out);
}

int psi_server_create_from_key(psi_server_buffer_t key_bytes,
                               bool reveal_intersection, psi_server_ctx *ctx,
                               char **error_out) {
  auto result = PsiServer::CreateFromKey(
      std::string(key_bytes.buff, key_bytes.buff_len), reveal_intersection);
  if (result.ok()) {
    *ctx = result->release();
    return 0;
  }
  return generate_error(result.status(), error_out);
}

void psi_server_delete(psi_server_ctx *ctx) {
  auto server = static_cast<PsiServer *>(*ctx);
  if (server == nullptr) {
    return;
  }
  delete server;
  *ctx = nullptr;
}

int psi_server_create_setup_message(
    psi_server_ctx ctx, double fpr, int64_t num_client_inputs,
    psi_server_buffer_t *input, size_t input_len, char **output,
    size_t *output_len, char **error_out,
    private_set_intersection::datastructure_t datastructure) {
  auto server = static_cast<PsiServer *>(ctx);
  if (server == nullptr) {
    return generate_error(absl::InvalidArgumentError("invalid server context"),
                          error_out);
  }

  std::vector<std::string> in;
  for (size_t idx = 0; idx < input_len; ++idx) {
    in.push_back(std::string(input[idx].buff, input[idx].buff_len));
  }

  auto result =
      server->CreateSetupMessage(fpr, num_client_inputs, in, datastructure);
  if (!result.ok()) {
    return generate_error(result.status(), error_out);
  }

  auto proto = std::move(*result);

  *output = (char *)malloc(proto.ByteSizeLong() * sizeof(char));
  if (*output == nullptr) {
    return generate_error(
        absl::InvalidArgumentError("failed to allocate memory"), error_out);
  }

  if (!proto.SerializeToArray(*output, proto.ByteSizeLong())) {
    return generate_error(
        absl::InvalidArgumentError("failed to serialize setup message"),
        error_out);
  }

  *output_len = proto.ByteSizeLong();
  return 0;
}

int psi_server_process_request(psi_server_ctx ctx,
                               psi_server_buffer_t client_request,
                               char **output, size_t *output_len,
                               char **error_out) {
  auto server = static_cast<PsiServer *>(ctx);
  if (server == nullptr) {
    return generate_error(absl::InvalidArgumentError("invalid server context"),
                          error_out);
  }

  psi_proto::Request request_proto;
  if (!request_proto.ParseFromArray(client_request.buff,
                                    client_request.buff_len)) {
    return generate_error(
        absl::InvalidArgumentError("failed to parse client request"),
        error_out);
  }
  auto result = server->ProcessRequest(request_proto);
  if (!result.ok()) {
    return generate_error(result.status(), error_out);
  }

  auto proto = std::move(*result);
  *output = (char *)malloc(proto.ByteSizeLong() * sizeof(char));
  if (*output == nullptr) {
    return generate_error(
        absl::InvalidArgumentError("failed to allocate memory"), error_out);
  }

  if (!proto.SerializeToArray(*output, proto.ByteSizeLong())) {
    return generate_error(
        absl::InvalidArgumentError("failed to serialize server response"),
        error_out);
  }

  *output_len = proto.ByteSizeLong();
  return 0;
}

int psi_server_get_private_key_bytes(psi_server_ctx ctx, char **output,
                                     size_t *output_len, char **error_out) {
  auto server = static_cast<PsiServer *>(ctx);
  if (server == nullptr) {
    return generate_error(absl::InvalidArgumentError("invalid server context"),
                          error_out);
  }
  auto value = server->GetPrivateKeyBytes();
  size_t len = value.size();

  // Private keys are raw bytes -> Use std::copy_n instead of strncpy.
  *output = (char *)malloc(len * sizeof(char));
  std::copy_n(value.begin(), len, *output);
  *output_len = len;

  return 0;
}
