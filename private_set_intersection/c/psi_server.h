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

#ifndef PRIVATE_SET_INTERSECTION_C_PSI_SERVER_H_
#define PRIVATE_SET_INTERSECTION_C_PSI_SERVER_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "private_set_intersection/cpp/datastructure/datastructure.h"

#ifdef __cplusplus
extern "C" {
namespace {
using private_set_intersection::datastructure_t;
}  // namespace
#endif

typedef void *psi_server_ctx;

struct psi_server_buffer_t {
  const char *buff;
  size_t buff_len;
};

int psi_server_create_with_new_key(bool reveal_intersection,
                                   psi_server_ctx *ctx, char **error_out);
int psi_server_create_from_key(struct psi_server_buffer_t key_bytes,
                               bool reveal_intersection, psi_server_ctx *ctx,
                               char **error_out);
void psi_server_delete(psi_server_ctx *ctx);

int psi_server_create_setup_message(psi_server_ctx ctx, double fpr,
                                    int64_t num_client_inputs,
                                    struct psi_server_buffer_t *input,
                                    size_t input_len, char **output,
                                    size_t *output_len, char **error_out,
                                    datastructure_t ds);

int psi_server_process_request(psi_server_ctx ctx,
                               struct psi_server_buffer_t client_request,
                               char **output, size_t *output_len,
                               char **error_out);
int psi_server_get_private_key_bytes(psi_server_ctx ctx, char **output,
                                     size_t *output_len, char **error_out);

#ifdef __cplusplus
}
#endif

#endif  // PRIVATE_SET_INTERSECTION_C_PSI_SERVER_H_
