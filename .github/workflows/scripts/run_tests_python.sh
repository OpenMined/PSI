#!/bin/sh
set -e

if [ "${RUNNER_OS}" -eq "macOS" ]:
then
    bazel clean --expunge
fi

python -m pip install --upgrade pip
pip install -r private_set_intersection/python/requirements_dev.txt

# Python + Bazel
bazel test -c opt --test_output=all --test_timeout=900 //private_set_intersection/python:tests

# Python package
pip install .
python private_set_intersection/python/tests.py
