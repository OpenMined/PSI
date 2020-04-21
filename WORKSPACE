workspace(name = "psi_cardinality")

load(
    "@bazel_tools//tools/build_defs/repo:http.bzl",
    "http_archive",
)

http_archive(
    name = "com_tencent_rapidjson",
    build_file_content = """
cc_library(
    name = "json",
    hdrs = glob(["**"]),
    visibility = ["//visibility:public"],
)
    """,
    sha256 = "8e00c38829d6785a2dfb951bb87c6974fa07dfe488aa5b25deec4b8bc0f6a3ab",
    strip_prefix = "rapidjson-1.1.0/include",
    url = "https://github.com/Tencent/rapidjson/archive/v1.1.0.zip",
)

http_archive(
    name = "private_join_and_compute",
    sha256 = "64be17ff362ff0338be49fe28658df73cc539c1b0f1d84b957d4a567097929ca",
    strip_prefix = "private-join-and-compute-eaec47fa64619e9a6467630663c7af70a4eadfcc",
    url = "https://github.com/google/private-join-and-compute/archive/eaec47fa64619e9a6467630663c7af70a4eadfcc.zip",
)

http_archive(
    name = "com_google_absl",
    sha256 = "9ab2dbebf6f209d6680bd3088a837c07f0788852ab81700aef6e2f2a746a7acb",
    strip_prefix = "abseil-cpp-b35973e3e35cb1eccb086d6a549c253c49579474",
    url = "https://github.com/abseil/abseil-cpp/archive/b35973e3e35cb1eccb086d6a549c253c49579474.zip",
)

http_archive(
    name = "com_google_googletest",
    sha256 = "94c634d499558a76fa649edb13721dce6e98fb1e7018dfaeba3cd7a083945e91",
    strip_prefix = "googletest-release-1.10.0",
    url = "https://github.com/google/googletest/archive/release-1.10.0.zip",
)

http_archive(
    name = "boringssl",
    sha256 = "7fefc298fa2a60fc04761768c2a3ded048cf69cc058e1167819546ef9efed325",
    strip_prefix = "boringssl-38496d7d00af11364b0fdc9dbf8b181277fa5dab",
    url = "https://github.com/google/boringssl/archive/38496d7d00af11364b0fdc9dbf8b181277fa5dab.zip",
)

http_archive(
    name = "com_github_glog_glog",
    sha256 = "f28359aeba12f30d73d9e4711ef356dc842886968112162bc73002645139c39c",
    strip_prefix = "glog-0.4.0",
    urls = ["https://github.com/google/glog/archive/v0.4.0.tar.gz"],
)

http_archive(
    name = "com_github_gflags_gflags",
    sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
    strip_prefix = "gflags-2.2.2",
    urls = [
        "https://mirror.bazel.build/github.com/gflags/gflags/archive/v2.2.2.tar.gz",
        "https://github.com/gflags/gflags/archive/v2.2.2.tar.gz",
    ],
)

http_archive(
    name = "rules_proto",
    sha256 = "602e7161d9195e50246177e7c55b2f39950a9cf7366f74ed5f22fd45750cd208",
    strip_prefix = "rules_proto-97d8af4dc474595af3900dd85cb3a29ad28cc313",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/rules_proto/archive/97d8af4dc474595af3900dd85cb3a29ad28cc313.tar.gz",
        "https://github.com/bazelbuild/rules_proto/archive/97d8af4dc474595af3900dd85cb3a29ad28cc313.tar.gz",
    ],
)

load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")

rules_proto_dependencies()

rules_proto_toolchains()
