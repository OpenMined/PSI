#!/bin/sh

python -m pip install --upgrade pip
pip install setuptools wheel twine auditwheel

pip wheel . -w dist/ --no-deps
auditwheel repair dist/*.whl --plat $AUDITWHEEL_PLAT
twine upload --skip-existing wheelhouse/*
