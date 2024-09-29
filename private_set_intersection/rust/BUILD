load("@rules_rust//proto/protobuf:toolchain.bzl", "PROTO_COMPILE_DEPS")
load("@rules_rust//rust:defs.bzl", "rust_binary", "rust_doc", "rust_library", "rust_test")

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
        "//private_set_intersection/c:c_package",
        "//private_set_intersection/c:c_psi_client",
        "//private_set_intersection/c:c_psi_server",
        "//private_set_intersection/proto:psi_rust_proto",
        "@crates//:libc",
        "@crates//:protobuf",
    ] + PROTO_COMPILE_DEPS,
)

rust_test(
    name = "version",
    crate = ":psi",
    deps = [
        "@crates//:semver",
    ],
)

rust_test(
    name = "test",
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
        "@crates//:bencher",
    ],
)

rust_doc(
    name = "doc",
    crate = ":psi",
)
