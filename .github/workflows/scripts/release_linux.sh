#!/bin/bash

set -e

yum -y update
yum -y install epel-release
yum --disablerepo="*" --enablerepo="epel" list python3*
yum --disablerepo="*" --enablerepo="epel" install python3 python3-devel
curl -O https://bootstrap.pypa.io/get-pip.py
/usr/bin/python3 get-pip.py

yum -y install \
    curl bison flex \
    epel-release python python-devel python-pip \
    git unzip which \
    && yum clean all

bazel_version="3.7.1"
# Install Bazel version specified in .bazelversion
curl -sSOL https://github.com/bazelbuild/bazel/releases/download/${bazel_version}/bazel-${bazel_version}-installer-linux-x86_64.sh
bash -x -e bazel-${bazel_version}-installer-linux-x86_64.sh

# Python
python -m pip install --upgrade pip
pip install -r private_set_intersection/python/requirements_dev.txt
pip install setuptools wheel twine auditwheel

# Publish
pip wheel . -w dist/ --no-deps
auditwheel repair dist/*.whl --plat $AUDITWHEEL_PLAT
twine upload --skip-existing wheelhouse/*
