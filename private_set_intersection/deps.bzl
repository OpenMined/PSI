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

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("//private_set_intersection/javascript/toolchain:cc_toolchain_config.bzl", "emsdk_configure")
load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")
load("@io_bazel_rules_go//go:deps.bzl", "go_register_toolchains", "go_rules_dependencies")
load("@rules_pkg//:deps.bzl", "rules_pkg_dependencies")
load("@bazel_gazelle//:deps.bzl", "gazelle_dependencies")
load("@pybind11_bazel//:python_configure.bzl", "python_configure")
load("@io_bazel_rules_rust//rust:repositories.bzl", "rust_repositories")
load("@io_bazel_rules_rust//proto:repositories.bzl", "rust_proto_repositories")
load("@io_bazel_rules_rust//:workspace.bzl", "bazel_version")
load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")
load("@build_bazel_rules_apple//apple:repositories.bzl", "apple_rules_dependencies")
load("//third_party/cargo:crates.bzl", "raze_fetch_remote_crates")
load("@rules_proto_grpc//:repositories.bzl", "rules_proto_grpc_repos", "rules_proto_grpc_toolchains")
load("@rules_proto_grpc//python:repositories.bzl", rules_proto_grpc_python_repos = "python_repos")

def psi_deps():
    # General dependencies.
    if "private_join_and_compute" not in native.existing_rules():
        #TODO revert to the upstream repository when the https://github.com/google/private-join-and-compute/pull/21 is merged
        http_archive(
            name = "private_join_and_compute",
            sha256 = "219f7cff49841901f8d88a7f84c9c8a61e69b5eb308a8535835743093eb4b595",
            strip_prefix = "private-join-and-compute-ee2c581454fd895d9928fe27b7ba0d0ebfd8fda2",
            url = "https://github.com/schoppmp/private-join-and-compute/archive/ee2c581454fd895d9928fe27b7ba0d0ebfd8fda2.zip",
        )

    if "com_google_absl" not in native.existing_rules():
        http_archive(
            name = "com_google_absl",
            sha256 = "d29785bb94deaba45946d40bde5b356c66a4eb76505de0181ea9a23c46bc5ed4",
            strip_prefix = "abseil-cpp-592924480acf034aec0454160492a20bccdbdf3e",
            url = "https://github.com/abseil/abseil-cpp/archive/592924480acf034aec0454160492a20bccdbdf3e.zip",
        )

    if "com_google_googletest" not in native.existing_rules():
        http_archive(
            name = "com_google_googletest",
            sha256 = "94c634d499558a76fa649edb13721dce6e98fb1e7018dfaeba3cd7a083945e91",
            strip_prefix = "googletest-release-1.10.0",
            url = "https://github.com/google/googletest/archive/release-1.10.0.zip",
        )

    if "com_google_benchmark" not in native.existing_rules():
        http_archive(
            name = "com_google_benchmark",
            sha256 = "a9d41abe1bd45a707d39fdfd46c01b92e340923bc5972c0b54a48002a9a7cfa3",
            strip_prefix = "benchmark-8cead007830bdbe94b7cc259e873179d0ef84da6",
            url = "https://github.com/google/benchmark/archive/8cead007830bdbe94b7cc259e873179d0ef84da6.zip",
        )

    if "boringssl" not in native.existing_rules():
        http_archive(
            name = "boringssl",
            sha256 = "7fefc298fa2a60fc04761768c2a3ded048cf69cc058e1167819546ef9efed325",
            strip_prefix = "boringssl-38496d7d00af11364b0fdc9dbf8b181277fa5dab",
            url = "https://github.com/google/boringssl/archive/38496d7d00af11364b0fdc9dbf8b181277fa5dab.zip",
        )

    if "com_github_glog_glog" not in native.existing_rules():
        http_archive(
            name = "com_github_glog_glog",
            sha256 = "ec64c82f3c2cd5be25d18f52bcca2840c1b29cf3d109cd61149935838645817b",
            strip_prefix = "glog-381e349a5bc3fd858a84b80c48ac465ad79c4a71",
            urls = ["https://github.com/schoppmp/glog/archive/381e349a5bc3fd858a84b80c48ac465ad79c4a71.zip"],
        )

    if "com_github_gflags_gflags" not in native.existing_rules():
        http_archive(
            name = "com_github_gflags_gflags",
            sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
            strip_prefix = "gflags-2.2.2",
            urls = [
                "https://github.com/gflags/gflags/archive/v2.2.2.tar.gz",
            ],
        )

    # gRPC for PJC.
    grpc_deps()
    apple_rules_dependencies()

    # Language-specific dependencies.

    # Javascript
    # Make all files under submodules/emsdk/* visible to the toolchain. The files are
    # available as external/emsdk/emsdk/*
    emsdk_configure(name = "emsdk")

    # Python.
    # Configure python3 for pybind11.
    python_configure(name = "local_config_python")

    # Protobuf.
    rules_proto_grpc_repos()
    rules_proto_grpc_toolchains()
    rules_proto_grpc_python_repos()

    rules_proto_dependencies()
    rules_proto_toolchains()

    # Golang.
    go_rules_dependencies()

    go_register_toolchains()

    rules_pkg_dependencies()

    gazelle_dependencies()

    # Rust.
    raze_fetch_remote_crates()

    rust_repositories()

    rust_proto_repositories()

    bazel_version(name = "bazel_version")
