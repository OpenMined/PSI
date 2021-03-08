#include "private_set_intersection/c/psi_client.h"

#include <algorithm>

#include "private_set_intersection/c/internal_utils.h"
#include "private_set_intersection/cpp/psi_client.h"
#include "private_set_intersection/proto/psi.pb.h"

namespace {
using private_set_intersection::PsiClient;
using private_set_intersection::c_bindings_internal::generate_error;
}  // namespace

int psi_client_create_with_new_key(bool reveal_intersection,
                                   psi_client_ctx *ctx, char **error_out) {
  auto result = PsiClient::CreateWithNewKey(reveal_intersection);
  if (result.ok()) {
    *ctx = result->release();
    return 0;
  }

  return generate_error(result.status(), error_out);
}

int psi_client_create_from_key(psi_client_buffer_t key_bytes,
                               bool reveal_intersection, psi_client_ctx *ctx,
                               char **error_out) {
  auto result = PsiClient::CreateFromKey(
      std::string(key_bytes.buff, key_bytes.buff_len), reveal_intersection);
  if (result.ok()) {
    *ctx = result->release();
    return 0;
  }

  return generate_error(result.status(), error_out);
}

void psi_client_delete(psi_client_ctx *ctx) {
  auto client = static_cast<PsiClient *>(*ctx);
  if (client == nullptr) {
    return;
  }
  delete client;
  *ctx = nullptr;
}

int psi_client_create_request(psi_client_ctx ctx, psi_client_buffer_t *inputs,
                              size_t input_len, char **output, size_t *out_len,
                              char **error_out) {
  auto client = static_cast<PsiClient *>(ctx);
  if (client == nullptr) {
    return generate_error(absl::InvalidArgumentError("invalid client context"),
                          error_out);
  }
  std::vector<std::string> in;
  for (size_t idx = 0; idx < input_len; ++idx) {
    in.push_back(std::string(inputs[idx].buff, inputs[idx].buff_len));
  }

  auto result = client->CreateRequest(in);
  if (!result.ok()) {
    return generate_error(result.status(), error_out);
  }
  auto proto = std::move(*result);

  std::string value;

  *output = (char *)malloc(proto.ByteSizeLong() * sizeof(char));
  if (*output == nullptr) {
    return generate_error(
        absl::InvalidArgumentError("failed to allocate memory"), error_out);
  }

  if (!proto.SerializeToArray(*output, proto.ByteSizeLong())) {
    return generate_error(
        absl::InvalidArgumentError("failed to serialize protobuffer"),
        error_out);
  }

  *out_len = proto.ByteSizeLong();
  return 0;
}

int psi_client_get_intersection_size(psi_client_ctx ctx,
                                     struct psi_client_buffer_t server_setup,
                                     struct psi_client_buffer_t server_response,
                                     int64_t *out, char **error_out) {
  auto client = static_cast<PsiClient *>(ctx);
  if (client == nullptr) {
    return generate_error(absl::InvalidArgumentError("invalid client context"),
                          error_out);
  }

  psi_proto::ServerSetup server_setup_proto;
  if (!server_setup_proto.ParseFromArray(server_setup.buff,
                                         server_setup.buff_len)) {
    return generate_error(
        absl::InvalidArgumentError("failed to parse server setup"), error_out);
  }

  psi_proto::Response server_response_proto;
  if (!server_response_proto.ParseFromArray(server_response.buff,
                                            server_response.buff_len)) {
    return generate_error(
        absl::InvalidArgumentError("failed to parse server response"),
        error_out);
  }

  auto result =
      client->GetIntersectionSize(server_setup_proto, server_response_proto);
  if (!result.ok()) {
    return generate_error(result.status(), error_out);
  }
  if (out != nullptr) {
    *out = *result;
  }
  return 0;
}

int psi_client_get_intersection(psi_client_ctx ctx,
                                struct psi_client_buffer_t server_setup,
                                struct psi_client_buffer_t server_response,
                                int64_t **out, size_t *outlen,
                                char **error_out) {
  auto client = static_cast<PsiClient *>(ctx);
  if (client == nullptr) {
    return generate_error(absl::InvalidArgumentError("invalid client context"),
                          error_out);
  }
  psi_proto::ServerSetup server_setup_proto;
  if (!server_setup_proto.ParseFromArray(server_setup.buff,
                                         server_setup.buff_len)) {
    return generate_error(
        absl::InvalidArgumentError("failed to parse server setup"), error_out);
  }

  psi_proto::Response server_response_proto;
  if (!server_response_proto.ParseFromArray(server_response.buff,
                                            server_response.buff_len)) {
    return generate_error(
        absl::InvalidArgumentError("failed to parse server response"),
        error_out);
  }

  auto result =
      client->GetIntersection(server_setup_proto, server_response_proto);
  if (!result.ok()) {
    return generate_error(result.status(), error_out);
  }
  if (out != nullptr) {
    *outlen = result->size();
    *out = (int64_t *)malloc(result->size() * sizeof(int64_t));
    std::copy_n(result->begin(), result->size(), *out);
  }
  return 0;
}

int psi_client_get_private_key_bytes(psi_client_ctx ctx, char **output,
                                     size_t *output_len, char **error_out) {
  auto client = static_cast<PsiClient *>(ctx);
  if (client == nullptr) {
    return generate_error(absl::InvalidArgumentError("invalid client context"),
                          error_out);
  }
  auto value = client->GetPrivateKeyBytes();
  size_t len = value.size();

  // Private keys are raw bytes -> Use std::copy_n instead of strncpy.
  *output = (char *)malloc(len * sizeof(char));
  std::copy_n(value.begin(), len, *output);
  *output_len = len;

  return 0;
}
