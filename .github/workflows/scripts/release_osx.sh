#!/bin/sh

export MACOSX_DEPLOYMENT_TARGET=10.13

python -m pip install --upgrade pip
pip install setuptools wheel twine auditwheel

pip wheel . -w wheel/
twine upload --skip-existing wheel/*
