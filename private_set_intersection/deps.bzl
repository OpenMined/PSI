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
load("@rules_rust//rust:repositories.bzl", "rust_repositories")
load("@rules_rust//proto:repositories.bzl", "rust_proto_repositories")
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
            sha256 = "a9f2550c2856f17d94885856bf3bd319868e27efad6b8740e3ada3c57cf163e5",
            strip_prefix = "private-join-and-compute-8ef3c87da225307c5601e88828b0e0bccfb19ba1",
            url = "https://github.com/s0l0ist/private-join-and-compute/archive/8ef3c87da225307c5601e88828b0e0bccfb19ba1.zip",
        )

    if "com_google_absl" not in native.existing_rules():
        http_archive(
            name = "com_google_absl",
            sha256 = "b9f490fae1c0d89a19073a081c3c588452461e5586e4ae31bc50a8f36339135e",
            strip_prefix = "abseil-cpp-8c0b94e793a66495e0b1f34a5eb26bd7dc672db0",
            url = "https://github.com/abseil/abseil-cpp/archive/8c0b94e793a66495e0b1f34a5eb26bd7dc672db0.zip",
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
            sha256 = "f6f62c4a1fc9b0a2edb70c77cdb9011b605430eabb4dddbb14d60fb492aea7bb",
            strip_prefix = "benchmark-d572f4777349d43653b21d6c2fc63020ab326db2",
            url = "https://github.com/google/benchmark/archive/d572f4777349d43653b21d6c2fc63020ab326db2.zip",
        )

    if "boringssl" not in native.existing_rules():
        http_archive(
            name = "boringssl",
            sha256 = "7fefc298fa2a60fc04761768c2a3ded048cf69cc058e1167819546ef9efed325",
            strip_prefix = "boringssl-38496d7d00af11364b0fdc9dbf8b181277fa5dab",
            url = "https://github.com/google/boringssl/archive/38496d7d00af11364b0fdc9dbf8b181277fa5dab.zip",
        )

    if "com_github_google_glog" not in native.existing_rules():
        http_archive(
            name = "com_github_google_glog",
            sha256 = "44fdfe7e9590e73ee3b92519c2f166b20301f3e0f8092fc6d03e540de95a3be4",
            strip_prefix = "glog-b33e3bad4c46c8a6345525fd822af355e5ef9446",
            urls = ["https://github.com/google/glog/archive/b33e3bad4c46c8a6345525fd822af355e5ef9446.zip"],
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

    go_register_toolchains(version = "1.19")

    rules_pkg_dependencies()

    gazelle_dependencies()

    # Rust.
    raze_fetch_remote_crates()

    rust_repositories(edition="2018")

    rust_proto_repositories()

    # bazel_version(name = "bazel_version")
