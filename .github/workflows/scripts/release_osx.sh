#!/bin/sh

python -m pip install --upgrade pip
pip install -r private_set_intersection/python/requirements_dev.txt
pip install setuptools wheel twine auditwheel

pip wheel . -w wheel/
twine upload --skip-existing wheel/*
