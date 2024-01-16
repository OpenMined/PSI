load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")
load("@rules_python//python:packaging.bzl", "py_package", "py_wheel")
load("//tools:package.bzl", "VERSION_LABEL")
load("@pip_deps//:requirements.bzl", "requirement")

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
    visibility = ["//visibility:private"],
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
        requirement("pytest"),
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

# This rule builds the wheel, but we need to change the name of the .whl before
# we publish to PyPI using a separate publish script outside of bazel. The
# output that gets generated looks like the following:
# openmined.psi-1.0.0-INTERPRETER-ABI-PLATFORM.whl
#
# This rule also bundles protobuf, but it resides in an incorrect directy and
# there doesn't seem to be a way to prevent it getting added to the wheel.
py_wheel(
    name = "wheel",
    abi = "ABI",
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
    python_tag = "INTERPRETER",
    requires = ["protobuf>=3.20"],
    version = VERSION_LABEL,
    deps = [
        ":openmined_psi_pkg",
    ],
)

exports_files(["requirements.txt"])
