#!/bin/sh
set -e

# JavaScript
npm install

npm run build
npm run build:proto
npm run compile
npm run test
