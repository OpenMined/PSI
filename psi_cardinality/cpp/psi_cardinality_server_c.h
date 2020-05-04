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

#ifndef PSI_CARDINALITY_PSI_CARDINALITY_SERVER_C_H_
#define PSI_CARDINALITY_PSI_CARDINALITY_SERVER_C_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* psi_cardinality_server_ctx;

struct server_buffer_t {
    const char* buff;
    size_t buff_len;
};

psi_cardinality_server_ctx psi_cardinality_server_create_with_new_key();
psi_cardinality_server_ctx psi_cardinality_server_create_from_key(
    struct server_buffer_t key_bytes);
void psi_cardinality_server_delete(psi_cardinality_server_ctx* ctx);

int psi_cardinality_server_create_setup_message(psi_cardinality_server_ctx ctx,
                                                double fpr,
                                                int64_t num_client_inputs,
                                                struct server_buffer_t* input,
                                                size_t input_len, char** output,
                                                size_t* output_len);
void psi_cardinality_server_delete_buffer(psi_cardinality_server_ctx ctx,
                                          char** input);

int psi_cardinality_server_process_request(
    psi_cardinality_server_ctx ctx, struct server_buffer_t client_request,
    char** output, size_t* output_len);
int psi_cardinality_server_get_private_key_bytes(psi_cardinality_server_ctx ctx,
                                                 char** output,
                                                 size_t* output_len);

#ifdef __cplusplus
}
#endif

#endif  // PSI_CARDINALITY_PSI_CARDINALITY_SERVER_C_H_
