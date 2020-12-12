#!/bin/sh

python -m pip install --upgrade pip
pip install -r private_set_intersection/python/requirements_dev.txt
pip install setuptools wheel twine auditwheel

python3 setup.py build bdist_wheel --plat-name macosx_10_14_x86_64 --dist-dir wheel

#pip wheel . -w wheel/
twine upload --skip-existing wheel/*
