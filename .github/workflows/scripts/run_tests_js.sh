#!/bin/sh
set -e

if [ "${RUNNER_OS}" -eq "macOS" ]:
then
    bazel clean --expunge
fi

# JavaScript
npm install
npm run build
npm run build:proto
npm run compile
npm run test
