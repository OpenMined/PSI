#!/bin/sh
set -e

pip install .
python private_set_intersection/python/tests.py
