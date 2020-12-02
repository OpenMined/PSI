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
            sha256 = "602e7161d9195e50246177e7c55b2f39950a9cf7366f74ed5f22fd45750cd208",
            strip_prefix = "rules_proto-97d8af4dc474595af3900dd85cb3a29ad28cc313",
            urls = [
                "https://mirror.bazel.build/github.com/bazelbuild/rules_proto/archive/97d8af4dc474595af3900dd85cb3a29ad28cc313.tar.gz",
                "https://github.com/bazelbuild/rules_proto/archive/97d8af4dc474595af3900dd85cb3a29ad28cc313.tar.gz",
            ],
        )

    if "io_bazel_rules_go" not in native.existing_rules():
        http_archive(
            name = "io_bazel_rules_go",
            sha256 = "7b9bbe3ea1fccb46dcfa6c3f3e29ba7ec740d8733370e21cdc8937467b4a4349",
            urls = [
                "https://mirror.bazel.build/github.com/bazelbuild/rules_go/releases/download/v0.22.4/rules_go-v0.22.4.tar.gz",
                "https://github.com/bazelbuild/rules_go/releases/download/v0.22.4/rules_go-v0.22.4.tar.gz",
            ],
        )

    if "bazel_gazelle" not in native.existing_rules():
        BAZEL_GAZELLE_VER = "0.20.0"
        BAZEL_GAZELLE_SHA256 = "d8c45ee70ec39a57e7a05e5027c32b1576cc7f16d9dd37135b0eddde45cf1b10"
        http_archive(
            name = "bazel_gazelle",
            sha256 = BAZEL_GAZELLE_SHA256,
            urls = [
                "https://storage.googleapis.com/bazel-mirror/github.com/bazelbuild/bazel-gazelle/releases/download/v%s/bazel-gazelle-v%s.tar.gz" % (BAZEL_GAZELLE_VER, BAZEL_GAZELLE_VER),
                "https://github.com/bazelbuild/bazel-gazelle/releases/download/v%s/bazel-gazelle-v%s.tar.gz" % (BAZEL_GAZELLE_VER, BAZEL_GAZELLE_VER),
            ],
        )

    if "rules_pkg" not in native.existing_rules():
        RULES_PKG_VER = "0.2.6"
        RULES_PKG_SHA256 = "aeca78988341a2ee1ba097641056d168320ecc51372ef7ff8e64b139516a4937"
        http_archive(
            name = "rules_pkg",
            urls = [
                "https://github.com/bazelbuild/rules_pkg/releases/download/%s-1/rules_pkg-%s.tar.gz" % (RULES_PKG_VER, RULES_PKG_VER),
                "https://mirror.bazel.build/github.com/bazelbuild/rules_pkg/releases/download/%s/rules_pkg-%s.tar.gz" % (RULES_PKG_VER, RULES_PKG_VER),
            ],
            sha256 = RULES_PKG_SHA256,
        )

    if "pybind11_bazel" not in native.existing_rules():
        http_archive(
            name = "pybind11_bazel",
            strip_prefix = "pybind11_bazel-203508e14aab7309892a1c5f7dd05debda22d9a5",
            urls = ["https://github.com/pybind/pybind11_bazel/archive/203508e14aab7309892a1c5f7dd05debda22d9a5.zip"],
            sha256 = "75922da3a1bdb417d820398eb03d4e9bd067c4905a4246d35a44c01d62154d91",
        )

    if "pybind11" not in native.existing_rules():
        http_archive(
            name = "pybind11",
            build_file = "@pybind11_bazel//:pybind11.BUILD",
            strip_prefix = "pybind11-2.5.0",
            urls = ["https://github.com/pybind/pybind11/archive/v2.5.0.zip"],
            sha256 = "1859f121837f6c41b0c6223d617b85a63f2f72132bae3135a2aa290582d61520",
        )

    if "rules_python" not in native.existing_rules():
        http_archive(
            name = "rules_python",
            url = "https://github.com/bazelbuild/rules_python/archive/a0fbf98d4e3a232144df4d0d80b577c7a693b570.zip",
            strip_prefix = "rules_python-a0fbf98d4e3a232144df4d0d80b577c7a693b570",
            sha256 = "98c9b903f6e8fe20b7e56d19c4822c8c49a11b475bd4ec0ca6a564e8bc5d5fa2",
        )

    RULES_PYTHON_EXTERNAL_VERSION = "3aacabb928a710b10bff13d0bde49ceaade58f15"
    if "rules_python_external" not in native.existing_rules():
        http_archive(
            name = "rules_python_external",
            sha256 = "5a1d7e6e4bab49dcdd787694f0f5d52ac5debdfc1852981a89cc414e338d60dc",
            strip_prefix = "rules_python_external-{version}".format(version = RULES_PYTHON_EXTERNAL_VERSION),
            url = "https://github.com/dillon-giacoppo/rules_python_external/archive/{version}.zip".format(version = RULES_PYTHON_EXTERNAL_VERSION),
        )

    if "io_bazel_rules_rust" not in native.existing_rules():
        http_archive(
            name = "io_bazel_rules_rust",
            sha256 = "75b29ba47ff4ef81f48574d1109bb6612788212524afe99e21467c71c980baa5",
            strip_prefix = "rules_rust-8cfa049d478ad33e407d572e260e912bdb31796a",
            urls = [
                # Master branch as of 25/07/2020
                "https://github.com/bazelbuild/rules_rust/archive/8cfa049d478ad33e407d572e260e912bdb31796a.tar.gz",
            ],
        )

    if "bazel_skylib" not in native.existing_rules():
        http_archive(
            name = "bazel_skylib",
            sha256 = "e5d90f0ec952883d56747b7604e2a15ee36e288bb556c3d0ed33e818a4d971f2",
            strip_prefix = "bazel-skylib-1.0.2",
            url = "https://github.com/bazelbuild/bazel-skylib/archive/1.0.2.tar.gz",
        )

    # gRPC for PJC.
    if "com_github_grpc_grpc" not in native.existing_rules():
        http_archive(
            name = "com_github_grpc_grpc",
            sha256 = "2060769f2d4b0d3535ba594b2ab614d7f68a492f786ab94b4318788d45e3278a",
            strip_prefix = "grpc-1.33.2",
            urls = [
                "https://github.com/grpc/grpc/archive/v1.33.2.tar.gz",
            ],
        )

    # For gRPC.
    if "build_bazel_rules_apple" not in native.existing_rules():
        http_archive(
            name = "build_bazel_rules_apple",
            strip_prefix = "rules_apple-b869b0d3868d78a1d4ffd866ccb304fb68aa12c3",
            sha256 = "bdc8e66e70b8a75da23b79f1f8c6207356df07d041d96d2189add7ee0780cf4e",
            urls = [
                "https://storage.googleapis.com/grpc-bazel-mirror/github.com/bazelbuild/rules_apple/archive/b869b0d3868d78a1d4ffd866ccb304fb68aa12c3.tar.gz",
                "https://github.com/bazelbuild/rules_apple/archive/b869b0d3868d78a1d4ffd866ccb304fb68aa12c3.tar.gz",
            ],
        )
        
    if "build_bazel_apple_support" not in native.existing_rules():
        http_archive(
            name = "build_bazel_apple_support",
            urls = [
                "https://storage.googleapis.com/grpc-bazel-mirror/github.com/bazelbuild/apple_support/releases/download/0.7.1/apple_support.0.7.1.tar.gz",
                "https://github.com/bazelbuild/apple_support/releases/download/0.7.1/apple_support.0.7.1.tar.gz",
            ],
            sha256 = "122ebf7fe7d1c8e938af6aeaee0efe788a3a2449ece5a8d6a428cb18d6f88033",
        )
        
    if "upb" not in native.existing_rules():
        http_archive(
            name = "upb",
            sha256 = "7992217989f3156f8109931c1fc6db3434b7414957cb82371552377beaeb9d6c",
            strip_prefix = "upb-382d5afc60e05470c23e8de19b19fc5ad231e732",
            urls = [
                "https://storage.googleapis.com/grpc-bazel-mirror/github.com/protocolbuffers/upb/archive/382d5afc60e05470c23e8de19b19fc5ad231e732.tar.gz",
                "https://github.com/protocolbuffers/upb/archive/382d5afc60e05470c23e8de19b19fc5ad231e732.tar.gz",
            ],
        )