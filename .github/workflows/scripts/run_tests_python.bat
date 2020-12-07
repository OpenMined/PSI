echo on

python -m pip install --upgrade pip

bazel build //private_set_intersection/python:openmined_psi_bin --verbose_failures
dir bazel-bin\private_set_intersection\python\
