package(default_visibility = ["//visibility:public"])

load("@rules_rust//rust:defs.bzl", "rust_binary", "rust_doc", "rust_library", "rust_test")
load("@rules_rust//proto:toolchain.bzl", "PROTO_COMPILE_DEPS")

rust_library(
    name = "rust_psi",
    srcs = [
        "src/client.rs",
        "src/lib.rs",
        "src/server.rs",
    ],
    deps = [
        "//private_set_intersection/c:c_psi_server",
        "//private_set_intersection/c:c_psi_client",
        "//private_set_intersection/c:c_package",
        "//third_party/cargo/cargo:libc",
        "//private_set_intersection/proto:psi_rust_proto",
    ] + PROTO_COMPILE_DEPS,
)

rust_test(
    name = "rust_version",
    crate = ":rust_psi",
    deps = [
        "//third_party/cargo/cargo:semver",
    ],
)

rust_test(
    name = "rust_integration",
    srcs = ["tests/integration_test.rs"],
    deps = [
        ":rust_psi",
    ],
)

rust_binary(
    name = "rust_psi_bench",
    srcs = ["benches/psi_bench.rs"],
    deps = [
        ":rust_psi",
        "//third_party/cargo/cargo:bencher",
    ],
)

rust_doc(
    name = "rust_psi_doc",
    crate = ":rust_psi",
)
