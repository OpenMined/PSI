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
load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")

def psi_cardinality_deps():
    if "com_tencent_rapidjson" not in native.existing_rules():
        http_archive(
            name = "com_tencent_rapidjson",
            build_file_content = """
cc_library(
    name = "json",
    hdrs = glob(["**"]),
    visibility = ["//visibility:public"],
)
    """,
            sha256 = "e6fc99c7df7f29995838a764dd68df87b71db360f7727ace467b21b82c85efda",
            strip_prefix = "rapidjson-8f4c021fa2f1e001d2376095928fc0532adf2ae6/include",
            url = "https://github.com/Tencent/rapidjson/archive/8f4c021fa2f1e001d2376095928fc0532adf2ae6.zip",
        )

    if "private_join_and_compute" not in native.existing_rules():
        http_archive(
            name = "private_join_and_compute",
            sha256 = "64be17ff362ff0338be49fe28658df73cc539c1b0f1d84b957d4a567097929ca",
            strip_prefix = "private-join-and-compute-eaec47fa64619e9a6467630663c7af70a4eadfcc",
            url = "https://github.com/google/private-join-and-compute/archive/eaec47fa64619e9a6467630663c7af70a4eadfcc.zip",
        )

    if "com_google_absl" not in native.existing_rules():
        http_archive(
            name = "com_google_absl",
            sha256 = "9ab2dbebf6f209d6680bd3088a837c07f0788852ab81700aef6e2f2a746a7acb",
            strip_prefix = "abseil-cpp-b35973e3e35cb1eccb086d6a549c253c49579474",
            url = "https://github.com/abseil/abseil-cpp/archive/b35973e3e35cb1eccb086d6a549c253c49579474.zip",
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
            sha256 = "f28359aeba12f30d73d9e4711ef356dc842886968112162bc73002645139c39c",
            strip_prefix = "glog-0.4.0",
            urls = ["https://github.com/google/glog/archive/v0.4.0.tar.gz"],
        )

    if "com_github_gflags_gflags" not in native.existing_rules():
        http_archive(
            name = "com_github_gflags_gflags",
            sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
            strip_prefix = "gflags-2.2.2",
            urls = [
                "https://mirror.bazel.build/github.com/gflags/gflags/archive/v2.2.2.tar.gz",
                "https://github.com/gflags/gflags/archive/v2.2.2.tar.gz",
            ],
        )

    rules_proto_dependencies()

    rules_proto_toolchains()
