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

load("@rules_cc//cc:defs.bzl", "cc_library")
load("@io_bazel_rules_rust//rust:rust.bzl", "rust_binary", "rust_test")
load("@io_bazel_rules_rust//bindgen:bindgen.bzl", "rust_bindgen", "rust_bindgen_library")

cc_library(
    name = "simple",
    hdrs = ["src/bindings.hpp"],
)

rust_bindgen_library(
    name = "simple_bindgen",
    bindgen_flags = [
        "--whitelist-var=SIMPLE_.*",
    ],
    cc_lib = ":simple",
    header = "src/bindings.hpp",
)

rust_binary(
    name = "simple_example",
    srcs = ["src/main.rs"],
    # deps = ["@io_bazel_rules_rust//bindgen/raze:bindgen"],
    deps = [":simple_bindgen"],
)

rust_test(
    name = "simple_test",
    crate = ":simple_example",
)
