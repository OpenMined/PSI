"""
@generated
cargo-raze generated Bazel file.

DO NOT EDIT! Replaced on runs of cargo-raze
"""

load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")  # buildifier: disable=load
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")  # buildifier: disable=load
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")  # buildifier: disable=load

def raze_fetch_remote_crates():
    """This function defines a collection of repos and should be called in a WORKSPACE file"""
    maybe(
        http_archive,
        name = "raze__bencher__0_1_5",
        url = "https://crates.io/api/v1/crates/bencher/0.1.5/download",
        type = "tar.gz",
        sha256 = "7dfdb4953a096c551ce9ace855a604d702e6e62d77fac690575ae347571717f5",
        strip_prefix = "bencher-0.1.5",
        build_file = Label("//third_party/cargo/remote:BUILD.bencher-0.1.5.bazel"),
    )

    maybe(
        http_archive,
        name = "raze__libc__0_2_138",
        url = "https://crates.io/api/v1/crates/libc/0.2.138/download",
        type = "tar.gz",
        sha256 = "db6d7e329c562c5dfab7a46a2afabc8b987ab9a4834c9d1ca04dc54c1546cef8",
        strip_prefix = "libc-0.2.138",
        build_file = Label("//third_party/cargo/remote:BUILD.libc-0.2.138.bazel"),
    )

    maybe(
        http_archive,
        name = "raze__once_cell__1_16_0",
        url = "https://crates.io/api/v1/crates/once_cell/1.16.0/download",
        type = "tar.gz",
        sha256 = "86f0b0d4bf799edbc74508c1e8bf170ff5f41238e5f8225603ca7caaae2b7860",
        strip_prefix = "once_cell-1.16.0",
        build_file = Label("//third_party/cargo/remote:BUILD.once_cell-1.16.0.bazel"),
    )

    maybe(
        http_archive,
        name = "raze__proc_macro2__1_0_47",
        url = "https://crates.io/api/v1/crates/proc-macro2/1.0.47/download",
        type = "tar.gz",
        sha256 = "5ea3d908b0e36316caf9e9e2c4625cdde190a7e6f440d794667ed17a1855e725",
        strip_prefix = "proc-macro2-1.0.47",
        build_file = Label("//third_party/cargo/remote:BUILD.proc-macro2-1.0.47.bazel"),
    )

    maybe(
        http_archive,
        name = "raze__protobuf__3_2_0",
        url = "https://crates.io/api/v1/crates/protobuf/3.2.0/download",
        type = "tar.gz",
        sha256 = "b55bad9126f378a853655831eb7363b7b01b81d19f8cb1218861086ca4a1a61e",
        strip_prefix = "protobuf-3.2.0",
        build_file = Label("//third_party/cargo/remote:BUILD.protobuf-3.2.0.bazel"),
    )

    maybe(
        http_archive,
        name = "raze__protobuf_support__3_2_0",
        url = "https://crates.io/api/v1/crates/protobuf-support/3.2.0/download",
        type = "tar.gz",
        sha256 = "a5d4d7b8601c814cfb36bcebb79f0e61e45e1e93640cf778837833bbed05c372",
        strip_prefix = "protobuf-support-3.2.0",
        build_file = Label("//third_party/cargo/remote:BUILD.protobuf-support-3.2.0.bazel"),
    )

    maybe(
        http_archive,
        name = "raze__quote__1_0_21",
        url = "https://crates.io/api/v1/crates/quote/1.0.21/download",
        type = "tar.gz",
        sha256 = "bbe448f377a7d6961e30f5955f9b8d106c3f5e449d493ee1b125c1d43c2b5179",
        strip_prefix = "quote-1.0.21",
        build_file = Label("//third_party/cargo/remote:BUILD.quote-1.0.21.bazel"),
    )

    maybe(
        http_archive,
        name = "raze__semver__1_0_14",
        url = "https://crates.io/api/v1/crates/semver/1.0.14/download",
        type = "tar.gz",
        sha256 = "e25dfac463d778e353db5be2449d1cce89bd6fd23c9f1ea21310ce6e5a1b29c4",
        strip_prefix = "semver-1.0.14",
        build_file = Label("//third_party/cargo/remote:BUILD.semver-1.0.14.bazel"),
    )

    maybe(
        http_archive,
        name = "raze__syn__1_0_105",
        url = "https://crates.io/api/v1/crates/syn/1.0.105/download",
        type = "tar.gz",
        sha256 = "60b9b43d45702de4c839cb9b51d9f529c5dd26a4aff255b42b1ebc03e88ee908",
        strip_prefix = "syn-1.0.105",
        build_file = Label("//third_party/cargo/remote:BUILD.syn-1.0.105.bazel"),
    )

    maybe(
        http_archive,
        name = "raze__thiserror__1_0_37",
        url = "https://crates.io/api/v1/crates/thiserror/1.0.37/download",
        type = "tar.gz",
        sha256 = "10deb33631e3c9018b9baf9dcbbc4f737320d2b576bac10f6aefa048fa407e3e",
        strip_prefix = "thiserror-1.0.37",
        build_file = Label("//third_party/cargo/remote:BUILD.thiserror-1.0.37.bazel"),
    )

    maybe(
        http_archive,
        name = "raze__thiserror_impl__1_0_37",
        url = "https://crates.io/api/v1/crates/thiserror-impl/1.0.37/download",
        type = "tar.gz",
        sha256 = "982d17546b47146b28f7c22e3d08465f6b8903d0ea13c1660d9d84a6e7adcdbb",
        strip_prefix = "thiserror-impl-1.0.37",
        build_file = Label("//third_party/cargo/remote:BUILD.thiserror-impl-1.0.37.bazel"),
    )

    maybe(
        http_archive,
        name = "raze__unicode_ident__1_0_5",
        url = "https://crates.io/api/v1/crates/unicode-ident/1.0.5/download",
        type = "tar.gz",
        sha256 = "6ceab39d59e4c9499d4e5a8ee0e2735b891bb7308ac83dfb4e80cad195c9f6f3",
        strip_prefix = "unicode-ident-1.0.5",
        build_file = Label("//third_party/cargo/remote:BUILD.unicode-ident-1.0.5.bazel"),
    )
