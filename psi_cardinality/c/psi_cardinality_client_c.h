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

#ifndef PSI_CARDINALITY_PSI_CARDINALITY_CLIENT_C_H_
#define PSI_CARDINALITY_PSI_CARDINALITY_CLIENT_C_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void *psi_cardinality_client_ctx;

struct client_buffer_t {
  const char *buff;
  uint32_t buff_len;
};

int psi_cardinality_client_create(psi_cardinality_client_ctx *ctx,
                                  char **error_out);
void psi_cardinality_client_delete(psi_cardinality_client_ctx *ctx);
int psi_cardinality_client_create_request(psi_cardinality_client_ctx ctx,
                                          struct client_buffer_t *inputs,
                                          uint32_t input_len, char **output,
                                          uint32_t *out_len, char **error_out);
void psi_cardinality_client_delete_buffer(psi_cardinality_client_ctx ctx,
                                          char **request);
int psi_cardinality_client_process_response(psi_cardinality_client_ctx,
                                            const char *server_setup,
                                            const char *server_responsei,
                                            int64_t *out, char **error_out);

#ifdef __cplusplus
}
#endif

#endif // PSI_CARDINALITY_PSI_CARDINALITY_CLIENT_C_H_
