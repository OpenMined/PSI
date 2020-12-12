#!/bin/sh

python -m pip install --upgrade pip
pip install -r private_set_intersection/python/requirements_dev.txt
pip install setuptools wheel twine auditwheel

python setup.py build bdist_wheel --plat-name=macosx_10_14_x86_64

#pip wheel . -w wheel/
#twine upload --skip-existing wheel/*
twine upload dist/*.whl --skip-existing
