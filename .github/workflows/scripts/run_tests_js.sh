#!/bin/sh
set -e

bazel clean --expunge

# JavaScript
npm run submodule:update
npm run em:update
npm run em:init
npm install
npm run build
npm run build:proto
npm run test
