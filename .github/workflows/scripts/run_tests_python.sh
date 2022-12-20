#!/bin/sh
set -e

python -m pip install --upgrade pip
pip install -r private_set_intersection/python/requirements.txt

# Python + Bazel
bazel test -c opt --test_output=all //private_set_intersection/python:tests

# Python package
# pip install .
# python private_set_intersection/python/tests.py
