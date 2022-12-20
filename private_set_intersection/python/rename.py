# This file is used to rename the wheel generated via py_wheel with values
# determined at runtime
import sys, os, re
from packaging import tags


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

    # openmined.psi-1.0.0-INTERPRETER-ABI-PLATFORM.whl
    # INTERPRETER and ABI should be the same value
    outfile = re.sub(r"INTERPRETER", abi_tag, inputfile)
    outfile = re.sub(r"ABI", abi_tag, outfile)

    print("renaming ", inputfile, outfile)
    os.rename(inputfile, outfile)


if __name__ == "__main__":
    main()
