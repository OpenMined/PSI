#!/bin/sh
set -e

files=`find ./private_set_intersection/go  -name *.go`

for file in $files
do
    # Check format.
    gofmt -w $file
    golint -set_exit_status $file
    if [ $? -ne 0 ]
    then
        exit 1
    fi
done

# Print changes.
git diff
# Already well formated if 'git diff' doesn't output anything.
! ( git diff |  grep -q ^ ) || exit 1
