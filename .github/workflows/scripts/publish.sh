#!/bin/sh
set -e

python -m pip install --upgrade pip
pip install twine

# upload to PyPI
twine upload --skip-existing --repository testpypi bazel-bin/private_set_intersection/python/openmined.psi-*
