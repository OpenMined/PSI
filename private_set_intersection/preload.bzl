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
            sha256 = "ae013bf35bd23234d1dea46b079f1e05ba74ac0321423830119d3e787ec73483",
            urls = [
                "https://mirror.bazel.build/github.com/bazelbuild/rules_go/releases/download/v0.36.0/rules_go-v0.36.0.zip",
                "https://github.com/bazelbuild/rules_go/releases/download/v0.36.0/rules_go-v0.36.0.zip",
            ],
        )


    if "bazel_gazelle" not in native.existing_rules():
        http_archive(
            name = "bazel_gazelle",
            sha256 = "efbbba6ac1a4fd342d5122cbdfdb82aeb2cf2862e35022c752eaddffada7c3f3",
            urls = [
                "https://mirror.bazel.build/github.com/bazelbuild/bazel-gazelle/releases/download/v0.27.0/bazel-gazelle-v0.27.0.tar.gz",
                "https://github.com/bazelbuild/bazel-gazelle/releases/download/v0.27.0/bazel-gazelle-v0.27.0.tar.gz",
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

    if "pybind11_bazel" not in native.existing_rules():
        pybind11_bazel_rev = "26973c0ff320cb4b39e45bc3e4297b82bc3a6c09"
        http_archive(
            name = "pybind11_bazel",
            strip_prefix = "pybind11_bazel-" + pybind11_bazel_rev,
            urls = ["https://github.com/pybind/pybind11_bazel/archive/" + pybind11_bazel_rev + ".zip"],
            sha256 = "a5666d950c3344a8b0d3892a88dc6b55c8e0c78764f9294e806d69213c03f19d",
        )

    if "pybind11" not in native.existing_rules():
        http_archive(
            name = "pybind11",
            build_file = "@pybind11_bazel//:pybind11.BUILD",
            strip_prefix = "pybind11-2.6.0",
            urls = ["https://github.com/pybind/pybind11/archive/v2.6.0.zip"],
            sha256 = "c2ed3fc84db08f40a36ce1d03331624ed6977497b35dfed36a1423396928559a",
        )

    if "rules_python" not in native.existing_rules():
        http_archive(
            name = "rules_python",
            sha256 = "497ca47374f48c8b067d786b512ac10a276211810f4a580178ee9b9ad139323a",
            strip_prefix = "rules_python-0.16.1",
            url = "https://github.com/bazelbuild/rules_python/archive/refs/tags/0.16.1.tar.gz",
        )

    if "rules_rust" not in native.existing_rules():
        http_archive(
            name = "rules_rust",
            sha256 = "dd79bd4e2e2adabae738c5e93c36d351cf18071ff2acf6590190acf4138984f6",
            urls = [
                "https://github.com/bazelbuild/rules_rust/releases/download/0.14.0/rules_rust-v0.14.0.tar.gz"
            ],
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
            sha256 = "f94e6dddf74739ef5cb30f000e13a2a613f6ebfa5e63588305a71fce8a8a9911",
            urls = [
                "https://github.com/bazelbuild/rules_apple/releases/download/1.1.3/rules_apple.1.1.3.tar.gz",
            ],
        )

    if "build_bazel_apple_support" not in native.existing_rules():
        http_archive(
            name = "build_bazel_apple_support",
            urls = [
                "https://github.com/bazelbuild/apple_support/releases/download/1.3.2/apple_support.1.3.2.tar.gz",
            ],
            sha256 = "2e3dc4d0000e8c2f5782ea7bb53162f37c485b5d8dc62bb3d7d7fc7c276f0d00",
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
            urls = ["https://github.com/rules-proto-grpc/rules_proto_grpc/archive/4.3.0.tar.gz"],
            sha256 = "fb7fc7a3c19a92b2f15ed7c4ffb2983e956625c1436f57a3430b897ba9864059",
            strip_prefix = "rules_proto_grpc-4.3.0",
        )
