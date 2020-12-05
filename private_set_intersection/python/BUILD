load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")
load("@org_openmined_psi_python_deps//:requirements.bzl", "requirement")

pybind_extension(
    name = "_openmined_psi",
    srcs = ["psi_bindings.cpp"],
    copts = [
        "--std=c++17",
    ],
    features = ["-use_header_modules"],
    linkstatic = True,
    visibility = ["//visibility:private"],
    deps = [
        "//private_set_intersection/cpp:package",
        "//private_set_intersection/cpp:psi_client",
        "//private_set_intersection/cpp:psi_server",
        "//private_set_intersection/proto:psi_cc_proto",
    ],
)

cc_library(
    name = "pybind_wrapper",
    data = select({
        "@bazel_tools//src/conditions:windows": ["//private_set_intersection/python:_openmined_psi.dll"],
        "//conditions:default": ["//private_set_intersection/python:_openmined_psi.so"],
    }),
)

py_library(
    name = "openmined_psi",
    srcs = ["__init__.py"],
    data = ["//private_set_intersection/python:_openmined_psi.so"],
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
        "//private_set_intersection/python:openmined_psi",
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
        "//private_set_intersection/python:openmined_psi",
        requirement("pytest"),
        requirement("pytest-benchmark"),
    ],
)
