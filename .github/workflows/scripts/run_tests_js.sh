#!/bin/sh
set -e

# JavaScript
npm run submodule:update
npm run em:update
npm run em:init
npm install
npm run build
npm run test
