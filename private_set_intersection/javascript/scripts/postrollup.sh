#!/usr/bin/env bash
set -euo pipefail

# Copy necessary files to dist directory
cp -rf package.json private_set_intersection/javascript/dist
cp -rf private_set_intersection/javascript/README.md private_set_intersection/javascript/dist
cp -rf LICENSE private_set_intersection/javascript/dist
cp -rf CHANGES.md private_set_intersection/javascript/dist

# Change directory into dist and pack to get
# shorter deep import links.
# Ex: 
# import PSI from '@openmined/psi.js/client_wasm_node'
# import PSI from '@openmined/psi.js/server_js_web'
# import PSI from '@openmined/psi.js/combined_wasm_worker'
cd private_set_intersection/javascript/dist

# Pack
npm pack