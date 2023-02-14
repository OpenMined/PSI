load("@rules_rust//rust:defs.bzl", "rust_binary", "rust_doc", "rust_library", "rust_test")
load("@rules_rust//proto:toolchain.bzl", "PROTO_COMPILE_DEPS")

package(default_visibility = ["//visibility:public"])

rust_library(
    name = "psi",
    srcs = [
        "src/client.rs",
        "src/datastructure.rs",
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
    name = "version",
    crate = ":psi",
    deps = [
        "//third_party/cargo/cargo:semver",
    ],
)

rust_test(
    name = "integration",
    srcs = ["tests/integration_test.rs"],
    deps = [
        ":psi",
    ],
)

rust_binary(
    name = "bench",
    srcs = ["benches/bench.rs"],
    deps = [
        ":psi",
        "//third_party/cargo/cargo:bencher",
    ],
)

rust_doc(
    name = "doc",
    crate = ":psi",
)
