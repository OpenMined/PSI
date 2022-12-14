load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")

pybind_extension(
    name = "_openmined_psi",
    srcs = ["psi_bindings.cpp"],
    # copts = [
    #     "--std=c++17",
    # ],
    features = ["-use_header_modules"],
    linkstatic = True,
    visibility = ["//visibility:private"],
    deps = [
        "//private_set_intersection/cpp:package",
        "//private_set_intersection/cpp:psi_client",
        "//private_set_intersection/cpp:psi_server",
        "//private_set_intersection/proto:psi_cc_proto",
        "@com_google_absl//absl/status:statusor",
    ],
)

py_library(
    name = "openmined_psi",
    srcs = [
        "__init__.py",
    ],
    data = ["//private_set_intersection/python:_openmined_psi.so"],
    srcs_version = "PY3",
    visibility = ["//visibility:public"],
    deps = [
        "//private_set_intersection/proto:psi_python_proto",
    ],
)

py_test(
    name = "tests",
    srcs = ["tests.py"],
    # https://github.com/bazelbuild/bazel/issues/10076
    legacy_create_init = False,
    main = "tests.py",
    python_version = "PY3",
    srcs_version = "PY3",
    deps = [
        ":openmined_psi",
    ],
)

py_binary(
    name = "benchmarks",
    srcs = ["benchmarks.py"],
    # https://github.com/bazelbuild/bazel/issues/10076
    legacy_create_init = False,
    main = "benchmarks.py",
    python_version = "PY3",
    srcs_version = "PY3",
    deps = [
        ":openmined_psi",
    ],
)
