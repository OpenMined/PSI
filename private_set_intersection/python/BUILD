load("@psi_deps//:requirements.bzl", "requirement")
load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")
load("@python//3.10:defs.bzl", py_binary_3_10 = "py_binary", py_test_3_10 = "py_test")
load("@python//3.11:defs.bzl", py_binary_3_11 = "py_binary", py_test_3_11 = "py_test")
load("@python//3.8:defs.bzl", py_binary_3_8 = "py_binary", py_test_3_8 = "py_test")
load("@python//3.9:defs.bzl", py_binary_3_9 = "py_binary", py_test_3_9 = "py_test")
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

def _make_variable_tags_impl(ctx):
    vars = {}

    # Determine ABI and Python Tag based on Python version or toolchain (example logic)
    python_version = ctx.toolchains["//python:python_toolchain"].python_version
    if python_version.startswith("3.8"):
        vars["ABI"] = "cp38"
        vars["PYTHON_TAG"] = "cp38"
    elif python_version.startswith("3.9"):
        vars["ABI"] = "cp39"
        vars["PYTHON_TAG"] = "cp39"
    else:
        vars["ABI"] = "abi3"  # Fallback to abi3 if targeting a stable ABI

    # Dynamically set version based on your versioning strategy
    vars["VERSION"] = "0.99.0"

    return [platform_common.TemplateVariableInfo(vars)]

make_variable_tags = rule(
    attrs = {},
    doc = """Make variable tags to pass to a py_wheel rule.""",
    implementation = _make_variable_tags_impl,
)

# This rule builds the wheel, but we need to change the name of the .whl before
# we publish to PyPI using a separate publish script outside of bazel. The
# output that gets generated looks like the following:
# openmined.psi-1.0.0-INTERPRETER-ABI-PLATFORM.whl
#
# This rule also bundles protobuf, but it resides in an incorrect directy and
# there doesn't seem to be a way to prevent it getting added to the wheel.
py_wheel(
    name = "wheel",
    abi = "$(ABI)",
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
        "@bazel_tools//src/conditions:linux_x86_64": "LINUX_x86_64",
        "@bazel_tools//src/conditions:linux_aarch64": "LINUX_aarch64",
    }),
    python_requires = ">=3.8.0",
    python_tag = "$(PYTHON_TAG)",
    requires = ["protobuf>=3.20"],
    toolchains = [":make_variable_tags"],
    version = VERSION_LABEL,
    deps = [
        ":openmined_psi_pkg",
    ],
)
