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

#include "private_set_intersection/cpp/golomb.h"

#include <cmath>
#include <vector>
#include <algorithm>
#include <cstddef>

// TODO: make intrinsics portable across compilers

#define DIV_CEIL(a, b) (((a) + (b) - 1) / (b))

namespace private_set_intersection {

const size_t CHUNK_SIZE = sizeof(char) * 8;

std::string golomb_compress(
        std::string& bloom_filter) {
    auto it = bloom_filter.begin();
    size_t prev_idx = 0;
    double avg = 0.0;
    size_t count = 0;

    while (it != bloom_filter.end()) {
        auto curr = *it;

        while (curr != 0) {
            auto curr_idx = __builtin_ctz(curr) + (it - bloom_filter.begin()) * CHUNK_SIZE;

            avg += (double)(curr_idx - prev_idx);
            ++count;

            curr &= (curr - 1);
            prev_idx = curr_idx;
        }

        ++it;
    }

    std::string res;

    if (count == 0) {
        return res;
    }

    avg /= (double)count;
    auto prob = 1 / avg; // assume geometric distribution
    auto div = (char)std::max(0.0, std::round(-std::log2(-std::log2(1.0 - prob))));

    res.push_back(div);

    size_t res_idx = 0;
    it = bloom_filter.begin();
    prev_idx = 0;

    while (it != bloom_filter.end()) {
        auto curr = *it;

        while (curr != 0) {
            auto curr_idx = __builtin_ctz(curr) + (it - bloom_filter.begin()) * CHUNK_SIZE;

            auto delta = curr_idx - prev_idx;
            auto quotient = delta >> (size_t)div;
            auto remainder = delta & ((1 << (size_t)div) - 1);
            auto len = quotient + 1 + (size_t)div;

            res.resize(DIV_CEIL(res_idx + len, CHUNK_SIZE), 0);

            auto unary_end = res_idx + quotient;
            res[unary_end / CHUNK_SIZE] |= 1 << (unary_end % CHUNK_SIZE);

            auto binary_start = (unary_end + 1) % 8;
            size_t binary_idx = 0;
            size_t i = (unary_end + 1) / CHUNK_SIZE;

            while (binary_idx < div) {
                res[i] |= (char)((remainder >> binary_idx) << binary_start);
                binary_idx += CHUNK_SIZE - binary_start;
                binary_start = 0;
                i++;
            }

            res_idx += len;
            curr &= (curr - 1);
            prev_idx = curr_idx;
        }

        ++it;
    }

    return res;
}

std::string golomb_decompress(
        std::string& golomb_compressed) {
    std::string res;
    return res;
}

}  // namespace private_set_intersection
