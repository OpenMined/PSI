load("@psi_deps//:requirements.bzl", "requirement")
load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")
load("@python//3.10:defs.bzl", py_binary_3_10 = "py_binary", py_test_3_10 = "py_test")
load("@python//3.11:defs.bzl", py_binary_3_11 = "py_binary", py_test_3_11 = "py_test")
load("@python//3.8:defs.bzl", py_binary_3_8 = "py_binary", py_test_3_8 = "py_test")
load("@python//3.9:defs.bzl", py_binary_3_9 = "py_binary", py_test_3_9 = "py_test")
load("@python_abi//:abi.bzl", "python_abi")
load("@python_platform//:platform.bzl", "python_platform")
load("@rules_python//python:packaging.bzl", "py_package", "py_wheel")
load("//tools:package.bzl", "VERSION_LABEL")

package(default_visibility = ["//visibility:public"])

pybind_extension(
    name = "_openmined_psi",
    srcs = ["psi_bindings.cpp"],
    linkstatic = True,
    visibility = ["//visibility:private"],
    deps = [
        "//private_set_intersection/cpp:package",
        "//private_set_intersection/cpp:psi_client",
        "//private_set_intersection/cpp:psi_server",
        "//private_set_intersection/proto:psi_cc_proto",
        "@abseil-cpp//absl/status:statusor",
    ],
)

py_library(
    name = "openmined_psi",
    srcs = [
        "__init__.py",
    ],
    data = ["//private_set_intersection/python:_openmined_psi.so"],
    srcs_version = "PY3",
    visibility = ["//visibility:private"],
    deps = [
        "//private_set_intersection/proto:psi_python_proto",
        requirement("protobuf"),
    ],
)

py_test_3_8(
    name = "test_3_8",
    srcs = ["tests.py"],
    # https://github.com/bazelbuild/bazel/issues/10076
    legacy_create_init = False,
    main = "tests.py",
    srcs_version = "PY3",
    deps = [
        ":openmined_psi",
        requirement("pytest"),
    ],
)

py_test_3_9(
    name = "test_3_9",
    srcs = ["tests.py"],
    # https://github.com/bazelbuild/bazel/issues/10076
    legacy_create_init = False,
    main = "tests.py",
    srcs_version = "PY3",
    deps = [
        ":openmined_psi",
        requirement("pytest"),
    ],
)

py_test_3_10(
    name = "test_3_10",
    srcs = ["tests.py"],
    # https://github.com/bazelbuild/bazel/issues/10076
    legacy_create_init = False,
    main = "tests.py",
    srcs_version = "PY3",
    deps = [
        ":openmined_psi",
        requirement("pytest"),
    ],
)

py_test_3_11(
    name = "test_3_11",
    srcs = ["tests.py"],
    # https://github.com/bazelbuild/bazel/issues/10076
    legacy_create_init = False,
    main = "tests.py",
    srcs_version = "PY3",
    deps = [
        ":openmined_psi",
        requirement("pytest"),
    ],
)

py_binary_3_8(
    name = "benchmark_3_8",
    srcs = ["benchmarks.py"],
    # https://github.com/bazelbuild/bazel/issues/10076
    legacy_create_init = False,
    main = "benchmarks.py",
    srcs_version = "PY3",
    deps = [
        ":openmined_psi",
        requirement("pytest-benchmark"),
        requirement("pytest"),
    ],
)

py_binary_3_9(
    name = "benchmark_3_9",
    srcs = ["benchmarks.py"],
    # https://github.com/bazelbuild/bazel/issues/10076
    legacy_create_init = False,
    main = "benchmarks.py",
    srcs_version = "PY3",
    deps = [
        ":openmined_psi",
        requirement("pytest-benchmark"),
        requirement("pytest"),
    ],
)

py_binary_3_10(
    name = "benchmark_3_10",
    srcs = ["benchmarks.py"],
    # https://github.com/bazelbuild/bazel/issues/10076
    legacy_create_init = False,
    main = "benchmarks.py",
    srcs_version = "PY3",
    deps = [
        ":openmined_psi",
        requirement("pytest-benchmark"),
        requirement("pytest"),
    ],
)

py_binary_3_11(
    name = "benchmark_3_11",
    srcs = ["benchmarks.py"],
    # https://github.com/bazelbuild/bazel/issues/10076
    legacy_create_init = False,
    main = "benchmarks.py",
    srcs_version = "PY3",
    deps = [
        ":openmined_psi",
        requirement("pytest-benchmark"),
        requirement("pytest"),
    ],
)

py_package(
    name = "openmined_psi_pkg",
    visibility = ["//visibility:private"],
    deps = [
        ":openmined_psi",
    ],
)

py_wheel(
    name = "wheel",
    abi = python_abi(),
    author = "OpenMined",
    classifiers = [
        "Programming Language :: Python :: 3",
        "Operating System :: OS Independent",
        "Topic :: Security :: Cryptography",
    ],
    description_file = "DESCRIPTION.md",
    distribution = "openmined.psi",
    extra_distinfo_files = {
        "//:LICENSE": "LICENSE",
        "//private_set_intersection/python:README.md": "README",
    },
    homepage = "https://github.com/OpenMined/PSI",
    license = "Apache 2.0",
    platform = select({
        "@bazel_tools//src/conditions:windows_x64": "win_amd64",
        "@bazel_tools//src/conditions:windows_arm64": "win_arm64",
        "@bazel_tools//src/conditions:darwin_x86_64": "macosx_12_0_x86_64",
        "@bazel_tools//src/conditions:darwin_arm64": "macosx_12_0_arm64",
        "@bazel_tools//src/conditions:linux_x86_64": python_platform() + "_x86_64",
        "@bazel_tools//src/conditions:linux_aarch64": python_platform() + "_aarch64",
    }),
    python_requires = ">=3.8.0",
    python_tag = python_abi(),
    # requires = ["protobuf>=3.20"],
    toolchains = ["@rules_python//python:current_py_toolchain"],
    version = VERSION_LABEL,
    deps = [
        ":openmined_psi_pkg",
    ],
)
