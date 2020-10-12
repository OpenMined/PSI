"""
cargo-raze crate workspace functions

DO NOT EDIT! Replaced on runs of cargo-raze
"""
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")

def _new_http_archive(name, **kwargs):
    if not native.existing_rule(name):
        http_archive(name=name, **kwargs)

def _new_git_repository(name, **kwargs):
    if not native.existing_rule(name):
        new_git_repository(name=name, **kwargs)

def raze_fetch_remote_crates():

    _new_http_archive(
        name = "raze__bencher__0_1_5",
        url = "https://crates-io.s3-us-west-1.amazonaws.com/crates/bencher/bencher-0.1.5.crate",
        type = "tar.gz",
        strip_prefix = "bencher-0.1.5",
        build_file = Label("//third_party/cargo/remote:bencher-0.1.5.BUILD"),
    )

    _new_http_archive(
        name = "raze__libc__0_2_74",
        url = "https://crates-io.s3-us-west-1.amazonaws.com/crates/libc/libc-0.2.74.crate",
        type = "tar.gz",
        strip_prefix = "libc-0.2.74",
        build_file = Label("//third_party/cargo/remote:libc-0.2.74.BUILD"),
    )

    _new_http_archive(
        name = "raze__protobuf__2_8_2",
        url = "https://crates-io.s3-us-west-1.amazonaws.com/crates/protobuf/protobuf-2.8.2.crate",
        type = "tar.gz",
        strip_prefix = "protobuf-2.8.2",
        build_file = Label("//third_party/cargo/remote:protobuf-2.8.2.BUILD"),
    )

    _new_http_archive(
        name = "raze__semver__0_10_0",
        url = "https://crates-io.s3-us-west-1.amazonaws.com/crates/semver/semver-0.10.0.crate",
        type = "tar.gz",
        strip_prefix = "semver-0.10.0",
        build_file = Label("//third_party/cargo/remote:semver-0.10.0.BUILD"),
    )

    _new_http_archive(
        name = "raze__semver_parser__0_7_0",
        url = "https://crates-io.s3-us-west-1.amazonaws.com/crates/semver-parser/semver-parser-0.7.0.crate",
        type = "tar.gz",
        strip_prefix = "semver-parser-0.7.0",
        build_file = Label("//third_party/cargo/remote:semver-parser-0.7.0.BUILD"),
    )

