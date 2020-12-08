#!/bin/sh

python -m pip install --upgrade pip
pip install setuptools wheel twine auditwheel

pip wheel . -w wheel/
twine upload --skip-existing wheel/*
