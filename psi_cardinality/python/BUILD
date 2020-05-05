load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")

pybind_extension(
  name = "psi_cardinality_bindings",
  srcs = ["bindings.cpp"],
  deps = [
    "//psi_cardinality/cpp:psi_cardinality_client",
    "//psi_cardinality/cpp:psi_cardinality_server",
  ],
)
