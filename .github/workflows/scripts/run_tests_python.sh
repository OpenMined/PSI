#!/bin/sh
set -e

python -m pip install --upgrade pip
pip install -r private_set_intersection/python/requirements_dev.txt

# Python + Bazel
bazel test -c opt --cxxopt="-std=c++17" --test_output=all //private_set_intersection/python:tests

# Python package
# pip install .
# python private_set_intersection/python/tests.py
