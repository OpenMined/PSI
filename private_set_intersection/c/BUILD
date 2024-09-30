#
# Copyright 2020 the authors listed in CONTRIBUTORS.md
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "c_package",
    srcs = ["package.cpp"],
    hdrs = ["package.h"],
    deps = [
        "//private_set_intersection/cpp:package",
    ],
)

cc_library(
    name = "c_internal_utils",
    srcs = ["internal_utils.cpp"],
    hdrs = ["internal_utils.h"],
    visibility = ["//visibility:private"],
    deps = [
        "@abseil-cpp//absl/status:statusor",
    ],
)

cc_library(
    name = "c_psi_client",
    srcs = ["psi_client.cpp"],
    hdrs = ["psi_client.h"],
    deps = [
        ":c_internal_utils",
        "//private_set_intersection/cpp:psi_client",
        "//private_set_intersection/cpp/datastructure",
    ],
)

cc_library(
    name = "c_psi_server",
    srcs = ["psi_server.cpp"],
    hdrs = ["psi_server.h"],
    deps = [
        ":c_internal_utils",
        "//private_set_intersection/cpp:psi_server",
        "//private_set_intersection/cpp/datastructure",
    ],
)

cc_test(
    name = "c_integration_test",
    srcs = ["integration_test.cpp"],
    deps = [
        ":c_package",
        ":c_psi_client",
        ":c_psi_server",
        "//private_set_intersection/cpp/util:status_matchers",
        "@abseil-cpp//absl/container:flat_hash_set",
        "@abseil-cpp//absl/strings",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_binary(
    name = "c_psi_benchmark",
    srcs = ["psi_benchmark.cpp"],
    deps = [
        ":c_psi_client",
        ":c_psi_server",
        "@abseil-cpp//absl/strings",
        "@google_benchmark//:benchmark_main",
    ],
)
