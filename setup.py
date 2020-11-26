import os
import posixpath
import re
import shutil
import sys

from distutils import sysconfig
import setuptools
from setuptools.command import build_ext


HERE = os.path.dirname(os.path.abspath(__file__))
IS_WINDOWS = sys.platform.startswith("win")


def _get_version():
    """Parse the version string from __init__.py."""
    with open(os.path.join(HERE, "tools", "package.bzl")) as f:
        key = "VERSION_LABEL"
        try:
            version_line = next(line for line in f if line.startswith(key))
        except StopIteration:
            raise ValueError("VERSION not defined")
        else:
            ns = {}
            exec(version_line, ns)  # pylint: disable=exec-used
            return ns[key]


class BazelExtension(setuptools.Extension):
    """A C/C++ extension that is defined as a Bazel BUILD target."""

    def __init__(self, name, bazel_target):
        self.bazel_target = bazel_target
        self.relpath, self.target_name = posixpath.relpath(bazel_target, "//").split(":")
        setuptools.Extension.__init__(self, name, sources=[])


class BuildBazelExtension(build_ext.build_ext):
    """A command that runs Bazel to build a C/C++ extension."""

    def run(self):
        for ext in self.extensions:
            self.bazel_build(ext)
        build_ext.build_ext.run(self)

    def bazel_build(self, ext):
        with open("WORKSPACE", "r") as f:
            workspace_contents = f.read()

        with open("WORKSPACE", "w") as f:
            f.write(
                re.sub(
                    r'(?<=path = ").*(?=",  # May be overwritten by setup\.py\.)',
                    sysconfig.get_python_inc().replace(os.path.sep, posixpath.sep),
                    workspace_contents,
                )
            )

        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)

        bazel_argv = [
            "bazel",
            "build",
            ext.bazel_target,
            "--symlink_prefix=" + os.path.join(self.build_temp, "bazel-"),
            "--compilation_mode=" + ("dbg" if self.debug else "opt"),
        ]

        self.spawn(bazel_argv)

        shared_lib_ext = ".dll" if IS_WINDOWS else ".so"
        shared_lib = "_private_set_intersection_ext" + shared_lib_ext
        ext_bazel_bin_path = os.path.join(self.build_temp, "bazel-bin", ext.relpath, shared_lib)

        ext_dest_path = self.get_ext_fullpath(ext.name)
        ext_dest_dir = os.path.dirname(ext_dest_path)
        if not os.path.exists(ext_dest_dir):
            os.makedirs(ext_dest_dir)
        shutil.copyfile(ext_bazel_bin_path, ext_dest_path)


setuptools.setup(
    name="private_set_intersection",
    version=_get_version(),
    description="Private Set Intersection protocol based on ECDH and Bloom Filters.",
    keywords="privacy cryptography",
    url="https://github.com/OpenMined/PSI",
    python_requires=">=3.6",
    package_dir={"": "private_set_intersection/python"},
    packages=setuptools.find_packages("private_set_intersection/python"),
    cmdclass=dict(build_ext=BuildBazelExtension),
    ext_modules=[
        BazelExtension(
            "private_set_intersection_ext",
            "//private_set_intersection/python:private_set_intersection_ext",
        )
    ],
    zip_safe=False,
    classifiers=[
        "Programming Language :: Python :: 3",
        "Operating System :: OS Independent",
        "Topic :: Security :: Cryptography",
    ],
    license="Apache 2.0",
)
