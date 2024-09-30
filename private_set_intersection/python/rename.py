import os
import platform
import re
import shutil
import sys
import zipfile
from enum import Enum
from platform import python_version

from packaging import tags, version


class ABIVersion(Enum):
    MANY_LINUX_X_Y = "manylinux_" + str(platform.libc_ver()[1].replace(".", "_"))
    MANY_LINUX_2014 = "manylinux2014"
    MANY_LINUX_2010 = "manylinux2010"


# We rely on the bundled pip versions installed with python to determine the
# naming variant: https://github.com/pypa/manylinux
#
# NOTE: Order matters
python_versions = {
    version.parse("3.12.0"): ABIVersion.MANY_LINUX_X_Y,
    version.parse("3.11.0"): ABIVersion.MANY_LINUX_X_Y,
    version.parse("3.10.0"): ABIVersion.MANY_LINUX_X_Y,
    version.parse("3.9.5"): ABIVersion.MANY_LINUX_X_Y,
    version.parse("3.9.0"): ABIVersion.MANY_LINUX_2014,
    version.parse("3.8.10"): ABIVersion.MANY_LINUX_X_Y,
    version.parse("3.8.4"): ABIVersion.MANY_LINUX_2014,
    version.parse("3.8.0"): ABIVersion.MANY_LINUX_2010,  # Fails for aarch64
}


def rename_internal_files(extract_dir):
    # Traverse the directory and rename files and folders with underscores to use dots
    for root, dirs, files in os.walk(extract_dir):
        for filename in files:
            new_filename = re.sub(r"openmined_psi", "openmined.psi", filename)
            if new_filename != filename:
                os.rename(
                    os.path.join(root, filename), os.path.join(root, new_filename)
                )
        for dirname in dirs:
            new_dirname = re.sub(r"openmined_psi", "openmined.psi", dirname)
            if new_dirname != dirname:
                os.rename(os.path.join(root, dirname), os.path.join(root, new_dirname))


def process_wheel(inputfile, outfile):
    extract_dir = inputfile.rstrip(".whl") + "_extracted"

    # Step 1: Extract the .whl (since it's just a zip archive)
    with zipfile.ZipFile(inputfile, "r") as zip_ref:
        zip_ref.extractall(extract_dir)

    # Step 2: Rename internal files with underscores to use dots
    rename_internal_files(extract_dir)

    # Step 3: Re-zip the contents back into a wheel file
    shutil.make_archive(outfile.rstrip(".whl"), "zip", extract_dir)
    shutil.move(outfile.rstrip(".whl") + ".zip", outfile)

    # Step 4: Clean up the extracted directory
    shutil.rmtree(extract_dir)

    # Step 5: Remove the original .whl file if successful
    if os.path.exists(outfile):
        os.remove(inputfile)
        print(f"Successfully renamed and removed the original file: {inputfile}")
    else:
        print(f"Renaming failed, keeping the original file: {inputfile}")


def main():
    inputfile = ""
    for file in os.listdir("./bazel-bin/private_set_intersection/python"):
        if file.startswith("openmined_psi-") and file.endswith(".whl"):
            inputfile = os.path.join(
                "./bazel-bin/private_set_intersection/python", file
            )

    if not inputfile:
        print("NOT FOUND")
        sys.exit(1)

    interpreter_name = tags.interpreter_name()
    interpreter_version = tags.interpreter_version()
    abi_tag = interpreter_name + interpreter_version

    # openmined.psi-1.0.0-INTERPRETER-ABI-PLATFORM-ARCH.whl
    # INTERPRETER and ABI should be the same value
    outfile = re.sub(r"INTERPRETER", abi_tag, inputfile)
    outfile = re.sub(r"ABI", abi_tag, outfile)
    outfile = re.sub(r"openmined_psi", "openmined.psi", outfile)
    system = platform.system()

    # We rename the wheel depending on the version of python and glibc
    if system.lower() == "linux":
        current_version = version.parse(python_version())
        manylinux = ABIVersion.MANY_LINUX_X_Y.value
        for ver, ml in python_versions.items():
            if current_version >= ver:
                manylinux = ml.value
                break

        outfile = re.sub(r"LINUX", manylinux, outfile)

    print("Renaming wheel and its internal contents: ", inputfile, " -> ", outfile)
    process_wheel(inputfile, outfile)

    # Step 5: Rename the external wheel file (already done in process_wheel)
    print("Wheel renamed successfully")


if __name__ == "__main__":
    main()
