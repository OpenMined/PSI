load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")
load("@org_openmined_psi_python_deps//:requirements.bzl", "requirement")

pybind_extension(
    name = "_psi_cardinality",
    srcs = ["psi_cardinality.cpp"],
    features = ["-use_header_modules"],
    linkstatic = True,
    visibility = ["//visibility:private"],
    deps = [
        "//psi_cardinality/cpp:package",
        "//psi_cardinality/cpp:psi_cardinality_client",
        "//psi_cardinality/cpp:psi_cardinality_server",
    ],
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
    python_version = "PY3",
    srcs_version = "PY3",
    deps = [
        "//psi_cardinality/python:psi_cardinality",
        requirement("pytest"),
    ],
)

py_binary(
    name = "benchmarks",
    srcs = ["benchmarks.py"],
    main = "benchmarks.py",
    python_version = "PY3",
    srcs_version = "PY3",
    deps = [
        "//psi_cardinality/python:psi_cardinality",
        requirement("pytest"),
        requirement("pytest-benchmark"),
    ],
)
