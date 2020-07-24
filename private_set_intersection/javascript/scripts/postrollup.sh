#!/usr/bin/env bash
set -euo pipefail

# Copy necessary files to dist directory
cp -rf package.json private_set_intersection/javascript/dist
cp -rf private_set_intersection/javascript/README.md private_set_intersection/javascript/dist
cp -rf LICENSE private_set_intersection/javascript/dist

# Copy the changelog into the js directory so that it is visible from 
cp -rf CHANGES.md private_set_intersection/javascript

# Change directory into dist and pack to get
# shorter deep import links.
# Ex: 
# import PSI from '@openmined/psi.js/client/wasm/iife'
# import PSI from '@openmined/psi.js/server/js/cjs'
# import PSI from '@openmined/psi.js/combined/wasm/es'
cd private_set_intersection/javascript/dist

# Pack
npm pack