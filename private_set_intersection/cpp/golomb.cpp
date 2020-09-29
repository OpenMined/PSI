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

namespace private_set_intersection {

std::string golomb_compress(
        const std::string& bloom_filter) {
    auto it = bloom_filter.begin();
    size_t prev_idx = 0;
    double avg = 0.0;
    size_t count = 0;

    while (it != bloom_filter.end()) {
        auto curr = *it;

        while (curr != 0) {
            auto curr_idx = static_cast<size_t>(CTZ(static_cast<unsigned int>(curr))) + (it - bloom_filter.begin()) * CHAR_SIZE;

            avg += static_cast<double>(curr_idx - prev_idx);
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

    avg /= static_cast<double>(count);
    auto prob = 1 / avg; // assume geometric distribution
    auto div = static_cast<size_t>(std::max(0.0, std::round(-std::log2(-std::log2(1.0 - prob)))));

    res.push_back(static_cast<char>(div));
    static size_t RES_OFFSET = 1;

    size_t res_idx = 0;
    it = bloom_filter.begin();
    prev_idx = 0;

    while (it != bloom_filter.end()) {
        auto curr = *it;

        while (curr != 0) {
            auto curr_idx = static_cast<size_t>(CTZ(static_cast<unsigned int>(curr))) + (it - bloom_filter.begin()) * CHAR_SIZE;

            auto delta = curr_idx - prev_idx;
            auto quotient = delta >> div;
            auto remainder = delta & ((1 << div) - 1);
            auto len = quotient + 1 + div;

            res.resize(RES_OFFSET + DIV_CEIL(res_idx + len, CHAR_SIZE), 0);

            auto unary_end = res_idx + quotient;
            res[RES_OFFSET + unary_end / CHAR_SIZE] |= static_cast<char>(1 << (unary_end % CHAR_SIZE));

            auto binary_start = (unary_end + 1) % 8;
            size_t binary_idx = 0;
            size_t i = (unary_end + 1) / CHAR_SIZE;

            while (binary_idx < div) {
                res[RES_OFFSET + i] |= static_cast<char>((remainder >> binary_idx) << binary_start);
                binary_idx += CHAR_SIZE - binary_start;
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
        const std::string& golomb_compressed) {
    if (golomb_compressed.empty()) {
        return "";
    }

    auto it = golomb_compressed.begin();
    auto div = static_cast<size_t>(*it);
    ++it;

    std::string res;
    size_t prefix_sum = 0;

    size_t offset = 0;

    while (true) {
        size_t quotient = 0;

        while (it != golomb_compressed.end() && (static_cast<unsigned char>(*it) >> offset) == 0) {
            quotient += CHAR_SIZE - offset;
            offset = 0;
            ++it;
        }

        if (it == golomb_compressed.end()) {
            break;
        }

        auto ctz = static_cast<size_t>(CTZ(static_cast<unsigned int>(static_cast<unsigned char>(*it >> offset))));
        quotient += ctz;
        auto one_idx = ctz + offset;
        auto binary_start = (one_idx + 1) % CHAR_SIZE;
        it += static_cast<size_t>(binary_start == 0);
        size_t binary_idx = 0;
        size_t remainder = 0;

        while (binary_idx < div) {
            auto num_bits = std::min(CHAR_SIZE - binary_start, div - binary_idx);
            remainder |= (static_cast<size_t>(static_cast<unsigned char>(*it) >> binary_start) & ((1 << num_bits) - 1)) << binary_idx;
            binary_idx += num_bits;
            binary_start = 0;
            ++it;
        }

        offset = (one_idx + 1 + div) % CHAR_SIZE;
        it -= static_cast<size_t>(offset != 0);

        auto delta = (quotient << div) | remainder;
        prefix_sum += delta;
        res.resize(prefix_sum / CHAR_SIZE + 1, 0);
        res[prefix_sum / CHAR_SIZE] |= static_cast<char>(1 << (prefix_sum % CHAR_SIZE));
    }

    return res;
}

}  // namespace private_set_intersection
