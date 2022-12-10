package(default_visibility = ["//visibility:public"])

load("@rules_rust//rust:defs.bzl", "rust_binary", "rust_doc", "rust_library", "rust_test")
load("@rules_rust//proto:toolchain.bzl", "PROTO_COMPILE_DEPS")

# This is a workaround for https://github.com/bazelbuild/rules_rust/issues/385.
# The issue is that the rust linker only finds the first library with any given name.
# Since PJC contains a legacy status library, absl/status is not found. This genrule renames
# absl/status, so it can be added as an additional dependency :absl_status to :rust_psi.
genrule(
    name = "rename_absl_status",
    srcs = ["@com_google_absl//absl/status"],
    outs = ["libabsl_status.a"],
    cmd = """
for file in $(locations @com_google_absl//absl/status); do
    if [[ $$(basename $$file) = "libstatus.a" ]]; then
        cp "$$file" "$(@D)"/libabsl_status.a;
    fi;
done
""",
)

cc_library(
    name = "absl_status",
    srcs = [":libabsl_status.a"],
)

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
        ":absl_status",
    ] + PROTO_COMPILE_DEPS,
)

rust_test(
    name = "rust_psi_unit_test",
    crate = ":rust_psi",
    deps = [
        "//third_party/cargo/cargo:semver",
    ],
)

rust_test(
    name = "rust_psi_test",
    srcs = ["tests/client_server_test.rs"],
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
