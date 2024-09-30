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

#ifndef PRIVATE_SET_INTERSECTION_CPP_GOLOMB_H_
#define PRIVATE_SET_INTERSECTION_CPP_GOLOMB_H_

#include <string>
#include <utility>
#include <vector>

namespace private_set_intersection {

const int64_t CHAR_SIZE = sizeof(char) * 8;

#if defined(_MSC_VER)  // MSVC
#include <intrin.h>
#pragma intrinsic(_BitScanForward)
__forceinline static int bsf(unsigned int x) {
  unsigned long i;
  _BitScanForward(&i, (unsigned long)x);
  return i;
}
#define CTZ(x) bsf(x)
#else  // GCC, Clang, etc.
#define CTZ(x) __builtin_ctz(x)
#endif

#define DIV_CEIL(a, b) (((a) + (b) - 1) / (b))

struct GolombCompressed {
  int64_t div;
  std::string compressed;
};

GolombCompressed golomb_compress(const std::vector<int64_t>& sorted_arr,
                                 int div_param = -1);

std::vector<int64_t> golomb_intersect(
    const std::string& golomb_compressed, int64_t div,
    const std::vector<std::pair<int64_t, int64_t>>& sorted_arr);

}  // namespace private_set_intersection

#endif  // PRIVATE_SET_INTERSECTION_CPP_GOLOMB_H_
