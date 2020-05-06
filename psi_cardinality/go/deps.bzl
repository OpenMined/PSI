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

def psi_cardinality_deps_go():
    RULES_GO_VER = "0.22.4"
    RULES_GO_SHA256 = "7b9bbe3ea1fccb46dcfa6c3f3e29ba7ec740d8733370e21cdc8937467b4a4349"

    if "io_bazel_rules_go" not in native.existing_rules():
        http_archive(
            name = "io_bazel_rules_go",
            sha256 = RULES_GO_SHA256,
            urls = [
                "https://mirror.bazel.build/github.com/bazelbuild/rules_go/releases/download/v%s/rules_go-v%s.tar.gz" % (RULES_GO_VER, RULES_GO_VER),
                "https://github.com/bazelbuild/rules_go/releases/download/v%s/rules_go-v%s.tar.gz" % (RULES_GO_VER, RULES_GO_VER),
            ],
        )

    BAZEL_GAZELLE_VER = "0.20.0"
    BAZEL_GAZELLE_SHA256 = "d8c45ee70ec39a57e7a05e5027c32b1576cc7f16d9dd37135b0eddde45cf1b10"

    if "bazel_gazelle" not in native.existing_rules():
        http_archive(
            name = "bazel_gazelle",
            sha256 = BAZEL_GAZELLE_SHA256,
            urls = [
                "https://storage.googleapis.com/bazel-mirror/github.com/bazelbuild/bazel-gazelle/releases/download/v%s/bazel-gazelle-v%s.tar.gz" % (BAZEL_GAZELLE_VER, BAZEL_GAZELLE_VER),
                "https://github.com/bazelbuild/bazel-gazelle/releases/download/v%s/bazel-gazelle-v%s.tar.gz" % (BAZEL_GAZELLE_VER, BAZEL_GAZELLE_VER),
            ],
        )

    PROTOBUF_VER = "3.11.4"
    PROTOBUF_SHA256 = "a79d19dcdf9139fa4b81206e318e33d245c4c9da1ffed21c87288ed4380426f9"

    if "com_google_protobuf" not in native.existing_rules():
        http_archive(
            name = "com_google_protobuf",
            sha256 = PROTOBUF_SHA256,
            urls = [
                "https://github.com/protocolbuffers/protobuf/archive/v%s.tar.gz" % PROTOBUF_VER,
            ],
            strip_prefix = "protobuf-%s" % PROTOBUF_VER
        )


    if "rules_proto" not in native.existing_rules():
        http_archive(
            name = "rules_proto",
            sha256 = "2490dca4f249b8a9a3ab07bd1ba6eca085aaf8e45a734af92aad0c42d9dc7aaf",
            strip_prefix = "rules_proto-218ffa7dfa5408492dc86c01ee637614f8695c45",
            urls = [
                "https://mirror.bazel.build/github.com/bazelbuild/rules_proto/archive/218ffa7dfa5408492dc86c01ee637614f8695c45.tar.gz",
                "https://github.com/bazelbuild/rules_proto/archive/218ffa7dfa5408492dc86c01ee637614f8695c45.tar.gz",
            ],
        )
    RULES_PKG_VER = "0.2.6"
    RULES_PKG_SHA256 = "aeca78988341a2ee1ba097641056d168320ecc51372ef7ff8e64b139516a4937"

    if "rules_pkg" not in native.existing_rules():
        http_archive(
            name = "rules_pkg",
            urls = [
                "https://github.com/bazelbuild/rules_pkg/releases/download/%s-1/rules_pkg-%s.tar.gz" % (RULES_PKG_VER, RULES_PKG_VER),
                "https://mirror.bazel.build/github.com/bazelbuild/rules_pkg/releases/download/%s/rules_pkg-%s.tar.gz" % (RULES_PKG_VER, RULES_PKG_VER),
            ],
            sha256 = RULES_PKG_SHA256,
        )
