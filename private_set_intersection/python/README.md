# PSI - Python

Private Set Intersection protocol based on ECDH, Bloom Filters, and Golomb
Compressed Sets.

## Tests

```
bazel test -c opt --test_output=all //private_set_intersection/python:tests
```

## Benchmarks

```
bazel run -c opt --test_output=all //private_set_intersection/python:benchmarks
```

## Updating dependencies

Add any dependencies to `requirements.in`, then use `pip-compile` to update the
`requirements.txt` file.

## Publishing

We first build the wheel

```
bazel build -c opt //private_set_intersection/python:wheel
```

Then, rename the wheel using the `rename.py` script, and finally publish using `twine`.
