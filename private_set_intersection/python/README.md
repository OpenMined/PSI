# PSI - Python

Private Set Intersection protocol based on ECDH, Bloom Filters, and Golomb
Compressed Sets.

## Tests

```bash
bazel test --test_output=all //private_set_intersection/python:test_3_8
bazel test --test_output=all //private_set_intersection/python:test_3_9
bazel test --test_output=all //private_set_intersection/python:test_3_10
bazel test --test_output=all //private_set_intersection/python:test_3_11
```

## Benchmarks

```bash
bazel run -c opt --test_output=all //private_set_intersection/python:benchmark_3_8
bazel run -c opt --test_output=all //private_set_intersection/python:benchmark_3_9
bazel run -c opt --test_output=all //private_set_intersection/python:benchmark_3_10
bazel run -c opt --test_output=all //private_set_intersection/python:benchmark_3_11
```

## Updating dependencies

Add any dependencies to `requirements.in`, then run:

```bash
bazel run //private_set_intersection/python/requirements:requirements_3_8.update
bazel run //private_set_intersection/python/requirements:requirements_3_9.update
bazel run //private_set_intersection/python/requirements:requirements_3_10.update
bazel run //private_set_intersection/python/requirements:requirements_3_11.update
```

## Publishing

We first build the wheel

```bash
bazel build -c opt //private_set_intersection/python:wheel
```

Then, rename the wheel using the `rename.py` script, and finally publish using `twine`.
