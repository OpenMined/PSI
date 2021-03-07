#!/bin/bash

set -e

yum -y update
yum -y install epel-release
yum --disablerepo="*" --enablerepo="epel" list python3*

yum -y install \
    curl bison flex \
    epel-release python3 python3-devel python3-pip \
    git unzip whichi protobuf-devel \
    && yum clean all

bazel_version="4.0.0"
# Install Bazel version specified in .bazelversion
curl -sSOLv https://github.com/bazelbuild/bazel/releases/download/${bazel_version}/bazel-${bazel_version}-installer-linux-x86_64.sh
bash -x -e bazel-${bazel_version}-installer-linux-x86_64.sh

# Python
python3 -m pip install --upgrade pip
python3 -m pip install -r private_set_intersection/python/requirements_dev.txt
python3 -m pip install setuptools wheel twine auditwheel

export BAZEL_LINKLIBS=-l%:libstdc++.a
export LD_LIBRARY_PATH=/usr/local/gcc-8.3.0/lib64/:$LD_LIBRARY_PATH

# Testing
bazel test --incompatible_require_linker_input_cc_api=false --features=-supports_dynamic_linker //private_set_intersection/python/...

# Publish
python3 -m pip wheel . -w dist/ --no-deps
auditwheel repair dist/*.whl --plat $AUDITWHEEL_PLAT
twine upload --skip-existing wheelhouse/*
