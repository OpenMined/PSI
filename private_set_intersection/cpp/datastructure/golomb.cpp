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

#include "private_set_intersection/cpp/datastructure/golomb.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <utility>
#include <vector>

namespace private_set_intersection {

GolombCompressed golomb_compress(const std::vector<int64_t>& sorted_arr,
                                 int div_param) {
  if (sorted_arr.empty()) {
    struct GolombCompressed res;
    res.div = 0;
    res.compressed = "";
    return res;
  }

  // estimate median through calculated average
  // calculate the average delta, assuming that the false positive rate is very
  // low
  auto avg = static_cast<double>(sorted_arr[sorted_arr.size() - 1] + 1) /
             sorted_arr.size();
  auto prob = 1 / avg;  // assume geometric distribution of deltas
  int64_t div = div_param >= 0
                    ? static_cast<int64_t>(div_param)
                    : static_cast<int64_t>(std::max(
                          0.0, std::round(-std::log2(-std::log2(1.0 - prob)))));

  std::string compressed;
  int64_t res_idx = 0;
  auto it = sorted_arr.begin();
  int64_t prev = 0;
  bool start = true;

  while (it != sorted_arr.end()) {
    auto curr = *it;

    // skip duplicates
    if (start | (curr > prev)) {
      auto delta = curr - prev;
      // decompose difference into quotient and remainder
      // divide by 2^div
      auto quotient = delta >> div;
      auto remainder = delta & ((static_cast<int64_t>(1) << div) - 1);
      auto len = quotient + 1 + div;

      compressed.resize(DIV_CEIL(res_idx + len, CHAR_SIZE), 0);

      // unary representation is a sequence of 0s, followed by 1
      auto unary_end = res_idx + quotient;
      compressed[unary_end / CHAR_SIZE] |=
          static_cast<char>(static_cast<int64_t>(1) << (unary_end % CHAR_SIZE));

      auto binary_start = (unary_end + 1) % CHAR_SIZE;
      int64_t binary_idx = 0;
      int64_t i = (unary_end + 1) / CHAR_SIZE;

      // copy each byte of the remainder to the resulting string
      // this is represented in binary
      while (binary_idx < div) {
        compressed[i] |= static_cast<char>(
            (static_cast<uint64_t>(remainder) >> binary_idx) << binary_start);
        binary_idx += CHAR_SIZE - binary_start;
        binary_start = 0;
        ++i;
      }

      res_idx += len;
      prev = curr;
      start = false;
    }

    ++it;
  }

  struct GolombCompressed res;
  res.div = div;
  res.compressed = compressed;
  return res;
}

std::vector<int64_t> golomb_intersect(
    const std::string& golomb_compressed, int64_t div,
    const std::vector<std::pair<int64_t, int64_t>>& sorted_arr) {
  if (golomb_compressed.empty()) {
    return std::vector<int64_t>();
  }

  auto it = golomb_compressed.begin();
  auto arr_it = sorted_arr.begin();

  int64_t prefix_sum = 0;
  int64_t offset = 0;

  std::vector<int64_t> res;

  while (true) {
    int64_t quotient = 0;

    // skip empty bytes in the string, which contain the unary quotient
    while (it != golomb_compressed.end() &&
           (static_cast<unsigned char>(*it) >> offset) == 0) {
      quotient += CHAR_SIZE - offset;
      offset = 0;
      ++it;
    }

    if (it == golomb_compressed.end()) {
      break;
    }

    // get the position of the first 1 bit, which is the end of the unary
    // portion
    auto ctz = static_cast<int64_t>(CTZ(
        static_cast<unsigned int>(static_cast<unsigned char>(*it) >> offset)));
    quotient += ctz;
    auto one_idx = ctz + offset;
    // skip the 1 bit that signals the end of the unary quotient
    auto binary_start = (one_idx + 1) % CHAR_SIZE;
    it += static_cast<size_t>(binary_start == 0);
    int64_t binary_idx = 0;
    int64_t remainder = 0;

    // copy the bytes from the string to the remainder (represented with binary)
    while (binary_idx < div) {
      auto num_bits = std::min(CHAR_SIZE - binary_start, div - binary_idx);
      remainder |= (static_cast<int64_t>(static_cast<unsigned char>(*it) >>
                                         binary_start) &
                    ((static_cast<int64_t>(1) << num_bits) - 1))
                   << binary_idx;
      binary_idx += num_bits;
      binary_start = 0;
      ++it;
    }

    // if the while loop is executed, then we may have erroneously skipped a
    // byte
    offset = (one_idx + 1 + div) % CHAR_SIZE;
    it -= static_cast<size_t>((div > 0) & (offset != 0));

    // reconstruct the delta
    auto delta = (quotient << div) | remainder;
    prefix_sum += delta;

    // now, check if the current the other (sorted) set contains the current
    // prefix_sum

    while (arr_it != sorted_arr.end() && (*arr_it).first < prefix_sum) {
      ++arr_it;
    }

    while (arr_it != sorted_arr.end() && (*arr_it).first == prefix_sum) {
      // the other set should contain a mapping to the indexes before sorting
      res.push_back((*arr_it).second);
      ++arr_it;
    }

    if (arr_it == sorted_arr.end()) {
      break;
    }
  }

  return res;
}

}  // namespace private_set_intersection
