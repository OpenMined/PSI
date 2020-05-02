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

#ifndef BLOOM_FILTER_C_H_
#define BLOOM_FILTER_C_H_
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef void* bloom_filter_ctx;
bloom_filter_ctx bloom_filter_create(double fpr, int64_t max_elements);
bloom_filter_ctx bloom_filter_create_from_json(const char* encoded_filter);
void bloom_filter_delete(bloom_filter_ctx ctx);

void bloom_filter_add(bloom_filter_ctx ctx, const char* input);
void bloom_filter_add_array(bloom_filter_ctx ctx, const char** input,
                            size_t len);
uint8_t bloom_filter_check(const bloom_filter_ctx ctx, const char* input);
void bloom_filter_to_json(const bloom_filter_ctx ctx, char* out,
                          size_t out_max_len);
int bloom_filter_num_hash_functions(const bloom_filter_ctx ctx);

#ifdef __cplusplus
}
#endif

#endif  // BLOOM_FILTER_C_H_
