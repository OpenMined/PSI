# This file is used to rename the wheel generated via py_wheel with values
# determined at runtime
import sys, os, re, platform
from packaging import tags, version
from platform import python_version
from enum import Enum


class ABIVersion(Enum):
    MANY_LINUX_X_Y = "manylinux_" + platform.libc_ver()[1].replace(".", "_")
    MANY_LINUX_2014 = "manylinux2014"
    MANY_LINUX_2010 = "manylinux2010"


# In python 3.7+, insertion order is guaranteed
python_versions = {
    version.parse("3.10.0"): ABIVersion.MANY_LINUX_X_Y,
    version.parse("3.9.5"): ABIVersion.MANY_LINUX_X_Y,
    version.parse("3.9.0"): ABIVersion.MANY_LINUX_2014,
    version.parse("3.8.10"): ABIVersion.MANY_LINUX_X_Y,
    version.parse("3.8.4"): ABIVersion.MANY_LINUX_2014,
    version.parse("3.8.0"): ABIVersion.MANY_LINUX_2010,
}


def main():
    inputfile = ""
    for file in os.listdir("./bazel-bin/private_set_intersection/python"):
        if file.startswith("openmined.psi-") and file.endswith(".whl"):
            inputfile = os.path.join("./bazel-bin/private_set_intersection/python", file)

    if not inputfile:
        sys.exit(1)

    interpreter_name = tags.interpreter_name()
    interpreter_version = tags.interpreter_version()
    abi_tag = interpreter_name + interpreter_version

    # openmined.psi-1.0.0-INTERPRETER-ABI-PLATFORM-ARCH.whl
    # INTERPRETER and ABI should be the same value
    outfile = re.sub(r"INTERPRETER", abi_tag, inputfile)
    outfile = re.sub(r"ABI", abi_tag, outfile)
    system = platform.system()

    # We rename the wheel depending on the version of python and glibc
    if system.lower() == "linux":
        current_version = version.parse(python_version())
        manylinux = ABIVersion.MANY_LINUX_X_Y.value()
        for (ver, ml) in python_versions.items():
            if current_version >= ver:
                manylinux = ml.value()
                break

        outfile = re.sub(r"LINUX", manylinux, outfile)

    print("renaming ", inputfile, outfile)
    os.rename(inputfile, outfile)


if __name__ == "__main__":
    main()
