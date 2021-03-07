#!/bin/sh
set -e

if [ "${RUNNER_OS}" -eq "macOS" ]:
then
    bazel clean --expunge
fi

# JavaScript
npm run submodule:update
npm run em:update
npm run em:init
npm install
npm run build
npm run build:proto
npm run test
