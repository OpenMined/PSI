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

PSI_LINKOPTS = select({
    "@platforms//os:osx": [],
    "//conditions:default": [
        # Needed on some Linux systems. See also
        # https://github.com/google/cctz/issues/47
        # https://github.com/tensorflow/tensorflow/issues/15129
        "-lrt",
    ],
})

cc_library(
    name = "datastructure",
    hdrs = ["datastructure.h"],
)

cc_library(
    name = "golomb",
    srcs = ["golomb.cpp"],
    hdrs = ["golomb.h"],
    visibility = ["//visibility:private"],
)

cc_test(
    name = "golomb_test",
    srcs = ["golomb_test.cpp"],
    linkopts = PSI_LINKOPTS,
    deps = [
        ":golomb",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "gcs",
    srcs = ["gcs.cpp"],
    hdrs = ["gcs.h"],
    deps = [
        ":golomb",
        "//private_set_intersection/proto:psi_cc_proto",
        "@abseil-cpp//absl/memory",
        "@abseil-cpp//absl/status:statusor",
        "@abseil-cpp//absl/strings",
        "@abseil-cpp//absl/types:span",
        "@private_join_and_compute//private_join_and_compute/crypto:bn_util",
    ],
)

cc_test(
    name = "gcs_test",
    srcs = ["gcs_test.cpp"],
    linkopts = PSI_LINKOPTS,
    deps = [
        ":gcs",
        "//private_set_intersection/cpp/util:status_matchers",
        "//private_set_intersection/proto:psi_cc_proto",
        "@abseil-cpp//absl/container:flat_hash_set",
        "@abseil-cpp//absl/strings",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "bloom_filter",
    srcs = ["bloom_filter.cpp"],
    hdrs = ["bloom_filter.h"],
    deps = [
        "//private_set_intersection/proto:psi_cc_proto",
        "@abseil-cpp//absl/memory",
        "@abseil-cpp//absl/status:statusor",
        "@abseil-cpp//absl/strings",
        "@abseil-cpp//absl/types:span",
        "@private_join_and_compute//private_join_and_compute/crypto:bn_util",
    ],
)

cc_test(
    name = "bloom_filter_test",
    srcs = ["bloom_filter_test.cpp"],
    linkopts = PSI_LINKOPTS,
    deps = [
        ":bloom_filter",
        "//private_set_intersection/cpp/util:status_matchers",
        "//private_set_intersection/proto:psi_cc_proto",
        "@abseil-cpp//absl/strings",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "raw",
    srcs = ["raw.cpp"],
    hdrs = ["raw.h"],
    deps = [
        "//private_set_intersection/proto:psi_cc_proto",
        "@abseil-cpp//absl/memory",
        "@abseil-cpp//absl/status:statusor",
        "@abseil-cpp//absl/strings",
        "@abseil-cpp//absl/types:span",
        "@private_join_and_compute//private_join_and_compute/crypto:bn_util",
    ],
)

cc_test(
    name = "raw_test",
    srcs = ["raw_test.cpp"],
    linkopts = PSI_LINKOPTS,
    deps = [
        ":raw",
        "//private_set_intersection/cpp/util:status_matchers",
        "//private_set_intersection/proto:psi_cc_proto",
        "@abseil-cpp//absl/strings",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
