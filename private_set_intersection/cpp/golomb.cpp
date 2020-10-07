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

#include <algorithm>
#include <cmath>
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

  auto it = sorted_arr.begin();
  int64_t prev = 0;
  double avg = 0.0;
  int64_t count = 0;
  bool start = true;

  while (it != sorted_arr.end()) {
    auto curr = *it;

    if (start | (curr > prev)) {  // | instead of || to save a branch
      avg += static_cast<double>(curr - prev);
      ++count;
      prev = curr;
      start = false;
    }

    ++it;
  }

  std::string compressed;

  avg /= static_cast<double>(count);
  auto prob = 1 / avg;  // assume geometric distribution
  int64_t div = div_param >= 0
                    ? static_cast<int64_t>(div_param)
                    : static_cast<int64_t>(std::max(
                          0.0, std::round(-std::log2(-std::log2(1.0 - prob)))));

  int64_t res_idx = 0;
  it = sorted_arr.begin();
  prev = 0;
  start = true;

  while (it != sorted_arr.end()) {
    auto curr = *it;

    if (start | (curr > prev)) {
      auto delta = curr - prev;
      auto quotient = delta >> div;
      auto remainder = delta & ((static_cast<int64_t>(1) << div) - 1);
      auto len = quotient + 1 + div;

      compressed.resize(DIV_CEIL(res_idx + len, CHAR_SIZE), 0);

      auto unary_end = res_idx + quotient;
      compressed[unary_end / CHAR_SIZE] |=
          static_cast<char>(static_cast<int64_t>(1) << (unary_end % CHAR_SIZE));

      auto binary_start = (unary_end + 1) % CHAR_SIZE;
      int64_t binary_idx = 0;
      int64_t i = (unary_end + 1) / CHAR_SIZE;

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

    while (it != golomb_compressed.end() &&
           (static_cast<unsigned char>(*it) >> offset) == 0) {
      quotient += CHAR_SIZE - offset;
      offset = 0;
      ++it;
    }

    if (it == golomb_compressed.end()) {
      break;
    }

    auto ctz = static_cast<int64_t>(CTZ(
        static_cast<unsigned int>(static_cast<unsigned char>(*it) >> offset)));
    quotient += ctz;
    auto one_idx = ctz + offset;
    auto binary_start = (one_idx + 1) % CHAR_SIZE;
    it += static_cast<size_t>(binary_start == 0);
    int64_t binary_idx = 0;
    int64_t remainder = 0;

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

    offset = (one_idx + 1 + div) % CHAR_SIZE;
    it -= static_cast<size_t>((div > 0) & (offset != 0));

    auto delta = (quotient << div) | remainder;
    prefix_sum += delta;

    while (arr_it != sorted_arr.end() && (*arr_it).first < prefix_sum) {
      ++arr_it;
    }

    while (arr_it != sorted_arr.end() && (*arr_it).first == prefix_sum) {
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
