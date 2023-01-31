#!/bin/sh
set -e

pip install -r private_set_intersection/python/requirements.txt

# Python + Bazel
bazel test -c opt --test_output=all //private_set_intersection/python:tests
