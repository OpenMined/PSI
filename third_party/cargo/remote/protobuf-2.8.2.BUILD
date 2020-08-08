"""
cargo-raze crate build file.

DO NOT EDIT! Replaced on runs of cargo-raze
"""
package(default_visibility = [
  # Public for visibility by "@raze__crate__version//" targets.
  #
  # Prefer access through "//third_party/cargo", which limits external
  # visibility to explicit Cargo.toml dependencies.
  "//visibility:public",
])

licenses([
  "notice", # MIT from expression "MIT"
])

load(
    "@io_bazel_rules_rust//rust:rust.bzl",
    "rust_library",
    "rust_binary",
    "rust_test",
)

load(
    "@io_bazel_rules_rust//cargo:cargo_build_script.bzl",
    "cargo_build_script",
)

cargo_build_script(
    name = "protobuf_build_script",
    srcs = glob(["**/*.rs"]),
    crate_root = "build.rs",
    edition = "2015",
    deps = [
    ],
    rustc_flags = [
        "--cap-lints=allow",
    ],
    crate_features = [
    ],
    build_script_env = {
    },
    data = glob(["**"]),
    tags = ["cargo-raze"],
    version = "2.8.2",
    visibility = ["//visibility:private"],
)

# Unsupported target "coded_input_stream" with type "bench" omitted
# Unsupported target "coded_output_stream" with type "bench" omitted

rust_library(
    name = "protobuf",
    crate_type = "lib",
    deps = [
        ":protobuf_build_script",
    ],
    srcs = glob(["**/*.rs"]),
    crate_root = "src/lib.rs",
    edition = "2015",
    rustc_flags = [
        "--cap-lints=allow",
    ],
    version = "2.8.2",
    tags = ["cargo-raze"],
    crate_features = [
    ],
)

