#!/bin/sh
set -e

# Python + Bazel
bazel test --test_output=all //private_set_intersection/python:tests

# Python package
pip install .
python private_set_intersection/python/tests.py

