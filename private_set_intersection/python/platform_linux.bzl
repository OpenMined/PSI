platform_linux_bzl_template = """\
def python_platform_linux():
    return "{python_platform_linux}"
"""

# we reuse the PYTHON_BIN_PATH environment variable from pybind11 so that the
# ABI tag we detect is always compatible with the version of python that was
# used for the build
_PYTHON_BIN_PATH_ENV = "PYTHON_BIN_PATH"

def _get_python_bin(rctx):
    # Extract the python_version from rctx.modules
    python_version = rctx.attr.python_version

    # Validate the python_version
    if python_version != "2" and python_version != "3":
        fail("Invalid python_version: '{python_version}'. It must be either '2' or '3'.".format(python_version = python_version))

    python_bin = rctx.os.environ.get(_PYTHON_BIN_PATH_ENV)
    if python_bin != None:
        return python_bin

    python_bin = rctx.which("python" + python_version)
    if python_bin != None:
        return python_bin

    # Fail if no python_version is found
    fail("python_version tag is not defined in any module.")

def _get_python_platform(rctx, python_bin):
    result = rctx.execute([
        python_bin,
        "-c",
        "import platform;" +
        "assert platform.python_implementation() == 'CPython';" +
        "version = 'manylinux_' + str(platform.libc_ver()[1].replace('.', '_'));" +
        "print(version)",
    ])

    return result.stdout.splitlines()[0]

def _declare_python_platform_impl(rctx):
    python_bin = _get_python_bin(rctx)

    python_platform = _get_python_platform(rctx, python_bin)

    rctx.report_progress("Declaring repository: python_platform_linux")
    rctx.file("BUILD")
    rctx.file("platform_linux.bzl", platform_linux_bzl_template.format(python_platform_linux = python_platform))

platform_linux = repository_rule(
    implementation = _declare_python_platform_impl,
    attrs = {
        "python_version": attr.string(
            doc = "Python version for the build '2' or '3' accepted.",
            mandatory = True,
        ),
    },
)

def _platform_ext_impl(_rctx):
    platform_linux(name = "python_platform_linux", python_version = "3")

declare_python_platform_linux = module_extension(
    implementation = _platform_ext_impl,
)
