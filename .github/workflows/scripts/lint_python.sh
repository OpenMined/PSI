#!/bin/sh
set -e

flake8 --config=.flake8 .
black --check psi_cardinality/python

# exit-zero treats all errors as warnings.
flake8 . --count --exit-zero --max-complexity=10 --max-line-length=100 --statistics

