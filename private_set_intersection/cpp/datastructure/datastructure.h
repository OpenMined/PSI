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
#ifndef PRIVATE_SET_INTERSECTION_CPP_DATASTRUCTURE_H_
#define PRIVATE_SET_INTERSECTION_CPP_DATASTRUCTURE_H_

#ifdef __cplusplus
namespace private_set_intersection {
extern "C" {
#endif

typedef enum DataStructure {
  Raw = 0,
  Gcs = 1,
  BloomFilter = 2,
} datastructure_t;

#ifdef __cplusplus
}
}
#endif

#endif  // PRIVATE_SET_INTERSECTION_CPP_DATASTRUCTURE_H_
