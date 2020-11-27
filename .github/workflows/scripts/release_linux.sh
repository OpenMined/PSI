#!/bin/sh

# Bazel
repofile="vbatts-bazel-epel-7.repo"
curl -O https://copr.fedorainfracloud.org/coprs/vbatts/bazel/repo/epel-7/$repofile
if [ ! -f $repofile ] ;  then
    echo "Download error"
    exit 1
fi

/bin/cp $repofile /etc/yum.repos.d/
yum --enablerepo=vbatts-bazel list bazel
yumdownloader --enablerepo=vbatts-bazel  bazel
yum install bazel3

# Python
python -m pip install --upgrade pip
pip install -r private_set_intersection/python/requirements_dev.txt
pip install setuptools wheel twine auditwheel

# Publish
pip wheel . -w dist/ --no-deps
auditwheel repair dist/*.whl --plat $AUDITWHEEL_PLAT
twine upload --skip-existing wheelhouse/*
