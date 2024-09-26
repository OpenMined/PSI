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

def psi_preload():
    if "platforms" not in native.existing_rules():
        http_archive(
            name = "platforms",
            urls = [
                "https://mirror.bazel.build/github.com/bazelbuild/platforms/releases/download/0.0.10/platforms-0.0.10.tar.gz",
                "https://github.com/bazelbuild/platforms/releases/download/0.0.10/platforms-0.0.10.tar.gz",
            ],
            sha256 = "218efe8ee736d26a3572663b374a253c012b716d8af0c07e842e82f238a0a7ee",
        )

    if "rules_cc" not in native.existing_rules():
        http_archive(
            name = "rules_cc",
            urls = ["https://github.com/bazelbuild/rules_cc/releases/download/0.0.10/rules_cc-0.0.10.tar.gz"],
            sha256 = "65b67b81c6da378f136cc7e7e14ee08d5b9375973427eceb8c773a4f69fa7e49",
            strip_prefix = "rules_cc-0.0.10",
        )

    if "rules_proto" not in native.existing_rules():
        http_archive(
            name = "rules_proto",
            sha256 = "80d3a4ec17354cccc898bfe32118edd934f851b03029d63ef3fc7c8663a7415c",
            strip_prefix = "rules_proto-5.3.0-21.5",
            urls = [
                "https://github.com/bazelbuild/rules_proto/archive/refs/tags/5.3.0-21.5.tar.gz",
            ],
        )

    if "io_bazel_rules_go" not in native.existing_rules():
        http_archive(
            name = "io_bazel_rules_go",
            sha256 = "de7974538c31f76658e0d333086c69efdf6679dbc6a466ac29e65434bf47076d",
            urls = [
                "https://mirror.bazel.build/github.com/bazelbuild/rules_go/releases/download/v0.45.0/rules_go-v0.45.0.zip",
                "https://github.com/bazelbuild/rules_go/releases/download/v0.45.0/rules_go-v0.45.0.zip",
            ],
        )

    if "bazel_gazelle" not in native.existing_rules():
        http_archive(
            name = "bazel_gazelle",
            sha256 = "32938bda16e6700063035479063d9d24c60eda8d79fd4739563f50d331cb3209",
            urls = [
                "https://mirror.bazel.build/github.com/bazelbuild/bazel-gazelle/releases/download/v0.35.0/bazel-gazelle-v0.35.0.tar.gz",
                "https://github.com/bazelbuild/bazel-gazelle/releases/download/v0.35.0/bazel-gazelle-v0.35.0.tar.gz",
            ],
        )

    if "rules_pkg" not in native.existing_rules():
        http_archive(
            name = "rules_pkg",
            sha256 = "eea0f59c28a9241156a47d7a8e32db9122f3d50b505fae0f33de6ce4d9b61834",
            urls = [
                "https://mirror.bazel.build/github.com/bazelbuild/rules_pkg/releases/download/0.8.0/rules_pkg-0.8.0.tar.gz",
                "https://github.com/bazelbuild/rules_pkg/releases/download/0.8.0/rules_pkg-0.8.0.tar.gz",
            ],
        )
    pybind11_bazel_ver = "2.12.0"

    if "pybind11_bazel" not in native.existing_rules():
        http_archive(
            name = "pybind11_bazel",
            strip_prefix = "pybind11_bazel-" + pybind11_bazel_ver,
            urls = ["https://github.com/pybind/pybind11_bazel/archive/v" + pybind11_bazel_ver + ".zip"],
            sha256 = "a58c25c5fe063a70057fa20cb8e15f3bda19b1030305bcb533af1e45f36a4a55",
        )

    if "pybind11" not in native.existing_rules():
        http_archive(
            name = "pybind11",
            build_file = "@pybind11_bazel//:pybind11-BUILD.bazel",
            strip_prefix = "pybind11-" + pybind11_bazel_ver,
            urls = ["https://github.com/pybind/pybind11/archive/v" + pybind11_bazel_ver + ".zip"],
            sha256 = "411f77380c43798506b39ec594fc7f2b532a13c4db674fcf2b1ca344efaefb68",
        )

    if "rules_python" not in native.existing_rules():
        http_archive(
            name = "rules_python",
            sha256 = "ca77768989a7f311186a29747e3e95c936a41dffac779aff6b443db22290d913",
            strip_prefix = "rules_python-0.36.0",
            url = "https://github.com/bazelbuild/rules_python/releases/download/0.36.0/rules_python-0.36.0.tar.gz",
        )

    if "rules_rust" not in native.existing_rules():
        http_archive(
            name = "rules_rust",
            sha256 = "5c2b6745236f8ce547f82eeacbbcc81d736734cc8bd92e60d3e3cdfa6e167bb5",
            urls = ["https://github.com/bazelbuild/rules_rust/releases/download/0.15.0/rules_rust-v0.15.0.tar.gz"],
        )

    if "bazel_skylib" not in native.existing_rules():
        http_archive(
            name = "bazel_skylib",
            sha256 = "74d544d96f4a5bb630d465ca8bbcfe231e3594e5aae57e1edbf17a6eb3ca2506",
            urls = [
                "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.3.0/bazel-skylib-1.3.0.tar.gz",
                "https://github.com/bazelbuild/bazel-skylib/releases/download/1.3.0/bazel-skylib-1.3.0.tar.gz",
            ],
        )

    # gRPC for PJC.
    if "com_github_grpc_grpc" not in native.existing_rules():
        http_archive(
            name = "com_github_grpc_grpc",
            sha256 = "cdeb805385fba23242bf87073e68d590c446751e09089f26e5e0b3f655b0f089",
            strip_prefix = "grpc-1.49.2",
            urls = [
                "https://github.com/grpc/grpc/archive/v1.49.2.tar.gz",
            ],
        )

    # For gRPC.
    if "build_bazel_rules_apple" not in native.existing_rules():
        http_archive(
            name = "build_bazel_rules_apple",
            sha256 = "86025f64d723a66438787d089bea4a7bc387877229f927dcb72ee26a8db96917",
            url = "https://github.com/bazelbuild/rules_apple/releases/download/3.9.2/rules_apple.3.9.2.tar.gz",
        )

    if "build_bazel_apple_support" not in native.existing_rules():
        http_archive(
            name = "build_bazel_apple_support",
            sha256 = "02f7456e589471dcfc73f1b9ca7f2c2ea5ca4aea999674abea9aebe818700b6e",
            url = "https://github.com/bazelbuild/apple_support/releases/download/1.17.0/apple_support.1.17.0.tar.gz",
        )

    if "upb" not in native.existing_rules():
        http_archive(
            name = "upb",
            sha256 = "69f1faa4b513a26759ed1153f29df67fc3722561d7d1c07e7f10ed7b17d47140",
            strip_prefix = "upb-9e8914228337f5e4e725d7071e517c45e6dea435",
            urls = [
                "https://github.com/protocolbuffers/upb/archive/9e8914228337f5e4e725d7071e517c45e6dea435.tar.gz",
            ],
        )

    if "rules_proto_grpc" not in native.existing_rules():
        http_archive(
            name = "rules_proto_grpc",
            urls = ["https://github.com/rules-proto-grpc/rules_proto_grpc/archive/refs/tags/4.3.0.tar.gz"],
            sha256 = "fb7fc7a3c19a92b2f15ed7c4ffb2983e956625c1436f57a3430b897ba9864059",
            strip_prefix = "rules_proto_grpc-4.3.0",
        )

    if "build_bazel_rules_nodejs" not in native.existing_rules():
        http_archive(
            name = "build_bazel_rules_nodejs",
            sha256 = "dcc55f810142b6cf46a44d0180a5a7fb923c04a5061e2e8d8eb05ccccc60864b",
            urls = ["https://github.com/bazelbuild/rules_nodejs/releases/download/5.8.0/rules_nodejs-5.8.0.tar.gz"],
        )

    if "emsdk" not in native.existing_rules():
        http_archive(
            name = "emsdk",
            sha256 = "b8270749b99d8d14922d1831b93781a5560fba6f7bce65cd477fc1b6aa262535",
            strip_prefix = "emsdk-3.1.28/bazel",
            urls = ["https://github.com/emscripten-core/emsdk/archive/refs/tags/3.1.28.tar.gz"],
        )

    # Must use 3.20.1 because of https://github.com/protocolbuffers/protobuf-javascript/issues/127
    if "google_protobuf" not in native.existing_rules():
        http_archive(
            name = "google_protobuf",
            sha256 = "8b28fdd45bab62d15db232ec404248901842e5340299a57765e48abe8a80d930",
            strip_prefix = "protobuf-3.20.1",
            urls = ["https://github.com/protocolbuffers/protobuf/archive/refs/tags/v3.20.1.tar.gz"],
        )
