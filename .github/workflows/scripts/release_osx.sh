#!/bin/sh

export MACOSX_DEPLOYMENT_TARGET=12.0

python -m pip install --upgrade pip
pip install -r private_set_intersection/python/requirements_dev.txt
pip install setuptools wheel twine auditwheel

python3 setup.py build bdist_wheel --plat-name macosx_12_0_arm64 --dist-dir wheel
python3 setup.py build bdist_wheel --plat-name macosx_12_0_x86_64 --dist-dir wheel
twine upload --skip-existing wheel/*
