import os
import re
import setuptools


def read(fname):
    return open(os.path.join(os.path.dirname(__file__), fname)).read()


def find_version():
    version_file = read("psi-cardinality/version.py")
    version_re = r"__version__ = '(?P<version>.+)'"
    version = re.match(version_re, version_file).group('version')
    return version


setuptools.setup(
    name="psi-cardinality",
    version=find_version(),
    description="Private Set Intersection Cardinality protocol based on ECDH and Bloom Filters.",
    license="Apache-2.0",
    keywords="privacy cryptography",
    # long_description=read("README.md"),
    long_description_content_type="text/markdown",
    url="https://github.com/OpenMined/psi-cardinality",
    classifiers=[
        "Programming Language :: Python :: 3",
        "Operating System :: OS Independent",
        "Topic :: Security :: Cryptography",
    ],
)
