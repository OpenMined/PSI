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

#include "psi_cardinality/c/package_c.h"
#include "psi_cardinality/cpp/package.h"

using Package = psi_cardinality::Package;

namespace psi_cardinality {

const char* package_version() {
    return Package::version.c_str();
}

// constexpr char Package::version[];

}  // namespace psi_cardinality
