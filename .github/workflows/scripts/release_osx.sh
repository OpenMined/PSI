#!/bin/sh

export -u MACOSX_DEPLOYMENT_TARGET=10.13

python -m pip install --upgrade pip
pip install -r private_set_intersection/python/requirements_dev.txt
pip install setuptools wheel twine auditwheel

pip wheel . -w wheel/
twine upload --skip-existing wheel/*
