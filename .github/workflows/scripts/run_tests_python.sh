#!/bin/sh
set -e

if [ ${RUNNER_OS}=="macOS" ]:
then
    bazel clean --expunge
    export -u MACOSX_DEPLOYMENT_TARGET=10.13
fi

# Python + Bazel
bazel test --test_output=all --test_timeout=900 //private_set_intersection/python:tests

# Python package
pip install .
python private_set_intersection/python/tests.py
