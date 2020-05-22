load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")
load("@org_openmined_psi_python_deps//:requirements.bzl", "requirement")

pybind_extension(
  name = "_psi_cardinality",
  srcs = ["psi_cardinality.cpp"],
  visibility = ["//visibility:public"],
  features = ["-use_header_modules"],
  deps = [
    "//psi_cardinality/cpp:psi_cardinality_client",
    "//psi_cardinality/cpp:psi_cardinality_server",
    "//psi_cardinality/cpp:package",
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

py_test(
    name = "tests",
    srcs = ["tests.py"],
    main = "tests.py",
    deps = [
        "//psi_cardinality/python:psi_cardinality",
        requirement("pytest"),
    ],
    legacy_create_init = False,
    python_version = "PY3",
    srcs_version = "PY3",
)

py_test(
    name = "benchmarks",
    srcs = ["benchmarks.py"],
    main = "benchmarks.py",
    deps = [
        "//psi_cardinality/python:psi_cardinality",
        requirement("pytest"),
        requirement("pytest-benchmark"),
    ],
    legacy_create_init = False,
    python_version = "PY3",
    srcs_version = "PY3",
)
