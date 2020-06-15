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

#ifndef PRIVATE_SET_INTERSECTION_C_PSI_CLIENT_H_
#define PRIVATE_SET_INTERSECTION_C_PSI_CLIENT_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void *psi_client_ctx;

struct psi_client_buffer_t {
  const char *buff;
  size_t buff_len;
};

int psi_client_create_with_new_key(bool reveal_intersection,
                                   psi_client_ctx *ctx, char **error_out);
int psi_client_create_from_key(struct psi_client_buffer_t key_bytes,
                               bool reveal_intersection, psi_client_ctx *ctx,
                               char **error_out);
void psi_client_delete(psi_client_ctx *ctx);
int psi_client_create_request(psi_client_ctx ctx,
                              struct psi_client_buffer_t *inputs,
                              size_t input_len, char **output, size_t *out_len,
                              char **error_out);
int psi_client_get_intersection_size(psi_client_ctx ctx,
                                     struct psi_client_buffer_t server_setup,
                                     struct psi_client_buffer_t server_response,
                                     int64_t *out, char **error_out);
int psi_client_get_intersection(psi_client_ctx ctx,
                                struct psi_client_buffer_t server_setup,
                                struct psi_client_buffer_t server_response,
                                int64_t **out, size_t *out_len,
                                char **error_out);
int psi_client_get_private_key_bytes(psi_client_ctx ctx, char **output,
                                     size_t *output_len, char **error_out);
#ifdef __cplusplus
}
#endif

#endif  // PRIVATE_SET_INTERSECTION_C_PSI_CLIENT_H_
