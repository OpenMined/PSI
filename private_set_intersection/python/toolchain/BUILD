load("@rules_python//python:defs.bzl", "py_runtime", "py_runtime_pair")

package(default_visibility = ["//visibility:public"])

py_runtime(
    name = "python_3_runtime",
    files = ["@python_3_interpreter//:files"],
    interpreter = "@python_3_interpreter//:python_bin",
    python_version = "PY3",
    visibility = ["//visibility:public"],
)

py_runtime_pair(
    name = "py_runtime_pair",
    py2_runtime = None,
    py3_runtime = ":python_3_runtime",
)

toolchain(
    name = "python_toolchain",
    toolchain = ":py_runtime_pair",
    toolchain_type = "@bazel_tools//tools/python:toolchain_type",
)
