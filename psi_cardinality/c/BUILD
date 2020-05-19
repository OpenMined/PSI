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
load("//tools:package.bzl", "VERSION_LABEL")
load("//tools:common.bzl", "template_rule")

package(default_visibility = ["//visibility:public"])

template_rule(
    name = "package_c_h",
    src = "package_c.h.templ",
    out = "package_c.h",
    substitutions = {
        "@VERSION_LABEL@": "%s" % VERSION_LABEL,
    },
)

cc_library(
    name = "package_c",
    srcs = ["package_c.cpp"],
    hdrs = [":package_c_h"],
    deps = [
    "//psi_cardinality/cpp:package",
    ],
)


# cc_test(
#     name = "package_test",
#     srcs = ["package_test_c.cpp"],
#     deps = [
#         ":package",
#         "@com_google_googletest//:gtest_main",
#     ],
# )

cc_library(
    name = "internal_utils",
    srcs = ["internal_utils.cpp"],
    hdrs = ["internal_utils.h"],
    visibility = ["//visibility:private"],
    deps = [
        "@private_join_and_compute//util:status",
    ],
)

cc_library(
    name = "psi_cardinality_client",
    srcs = ["psi_cardinality_client.cpp"],
    hdrs = ["psi_cardinality_client.h"],
    deps = [
        "//psi_cardinality/c:internal_utils",
        "//psi_cardinality/cpp:psi_cardinality_client",
    ],
)

cc_test(
    name = "psi_cardinality_client_test",
    srcs = ["psi_cardinality_client_test.cpp"],
    deps = [
        ":psi_cardinality_client",
        "//psi_cardinality/cpp/util:status_matchers",
        "@com_google_absl//absl/strings",
        "@com_google_googletest//:gtest",
        "@com_google_googletest//:gtest_main",
        "@com_tencent_rapidjson//:rapidjson",
        "@private_join_and_compute//crypto:ec_commutative_cipher",
    ],
)

cc_library(
    name = "psi_cardinality_server",
    srcs = ["psi_cardinality_server.cpp"],
    hdrs = ["psi_cardinality_server.h"],
    deps = [
        "//psi_cardinality/c:internal_utils",
        "//psi_cardinality/cpp:psi_cardinality_server",
    ],
)

cc_test(
    name = "psi_cardinality_server_test",
    srcs = ["psi_cardinality_server_test.cpp"],
    deps = [
        ":psi_cardinality_client",
        ":psi_cardinality_server",
        "//psi_cardinality/cpp/util:status_matchers",
        "@com_google_absl//absl/strings",
        "@com_google_googletest//:gtest",
        "@com_google_googletest//:gtest_main",
        "@com_tencent_rapidjson//:rapidjson",
    ],
)

cc_binary(
    name = "psi_cardinality_benchmark",
    srcs = ["psi_cardinality_benchmark.cpp"],
    deps = [
        ":psi_cardinality_client",
        ":psi_cardinality_server",
        "@com_google_absl//absl/strings",
        "@com_google_benchmark//:benchmark_main",
    ],
)
