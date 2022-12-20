#!/bin/sh
set -e

# JavaScript
npm install

bazel build -c opt //private_set_intersection/javascript/cpp:psi_wasm_node.js
bazel build -c opt //private_set_intersection/javascript/cpp:psi_wasm_web.js
bazel build -c opt //private_set_intersection/javascript/cpp:psi_wasm_worker.js

npm run build:copy
npm run build:proto
npm run compile
npm run test
