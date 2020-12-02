echo on

pip install .
bazel test --test_output=all --cache_test_results=no //private_set_intersection/python:tests
