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
    name = "psi_cardinality_client_c",
    srcs = ["psi_cardinality_client_c.cpp", "internal_utils.h", "internal_utils.cpp"],
    hdrs = ["psi_cardinality_client_c.h"],
    includes = ["."],
    deps = [
        "//psi_cardinality/cpp:bloom_filter",
        "//psi_cardinality/cpp:psi_cardinality_client",
        "@boringssl//:crypto",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:span",
        "@com_tencent_rapidjson//:json",
        "@private_join_and_compute//crypto:ec_commutative_cipher",
        "@private_join_and_compute//util:status",
    ],
)

cc_test(
    name = "psi_cardinality_client_c_test",
    srcs = ["psi_cardinality_client_test_c.cpp"],
    deps = [
        ":psi_cardinality_client_c",
        "//psi_cardinality/cpp/util:status_matchers",
        "@com_google_absl//absl/strings",
        "@com_google_googletest//:gtest",
        "@com_google_googletest//:gtest_main",
        "@com_tencent_rapidjson//:json",
        "@private_join_and_compute//crypto:ec_commutative_cipher",
    ],
)

cc_library(
    name = "psi_cardinality_server_c",
    srcs = ["psi_cardinality_server_c.cpp", "internal_utils.h", "internal_utils.cpp"],
    hdrs = ["psi_cardinality_server_c.h"],
    includes = ["."],
    deps = [
        "//psi_cardinality/cpp:bloom_filter",
        "//psi_cardinality/cpp:psi_cardinality_server",
        "@boringssl//:crypto",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:span",
        "@com_tencent_rapidjson//:json",
        "@private_join_and_compute//crypto:ec_commutative_cipher",
        "@private_join_and_compute//util:status",
    ],
)

cc_test(
    name = "psi_cardinality_server_c_test",
    srcs = ["psi_cardinality_server_test_c.cpp"],
    deps = [
        ":psi_cardinality_client_c",
        ":psi_cardinality_server_c",
        "//psi_cardinality/cpp/util:status_matchers",
        "@com_google_absl//absl/strings",
        "@com_google_googletest//:gtest",
        "@com_google_googletest//:gtest_main",
        "@com_tencent_rapidjson//:json",
    ],
)

cc_binary(
    name = "psi_cardinality_benchmark_c",
    srcs = ["psi_cardinality_benchmark_c.cpp"],
    deps = [
        ":psi_cardinality_client_c",
        ":psi_cardinality_server_c",
        "@com_google_absl//absl/strings",
        "@com_google_benchmark//:benchmark_main",
    ],
)
