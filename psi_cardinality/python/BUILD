load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")

pybind_extension(
  name = "_psi_cardinality",
  srcs = ["psi_cardinality.cpp"],
  visibility = ["//visibility:public"],
  features = ["-use_header_modules"],
  deps = [
    "//psi_cardinality/cpp:psi_cardinality_client",
    "//psi_cardinality/cpp:psi_cardinality_server",
  ],
  linkstatic = True,
)

py_library(
    name = "psi_cardinality",
    srcs = ["__init__.py"],
    data = ["//psi_cardinality/python:_psi_cardinality.so"],
    srcs_version = "PY3",

    visibility = ["//visibility:public"],
)
