#!/usr/bin/env bash
set -euo pipefail

# Copy necessary files to dist directory
cp -rf package.json psi_cardinality/javascript/dist
cp -rf psi_cardinality/javascript/README.md psi_cardinality/javascript/dist
cp -rf LICENSE psi_cardinality/javascript/dist

# Change directory into dist and pack to get
# shorter deep import links.
# Ex: 
# import PSI from '@openmined/psi.js/client/wasm/es'
# import PSI from '@openmined/psi.js/server/js/umd'
# import PSI from '@openmined/psi.js/combined/wasm/es'
cd psi_cardinality/javascript/dist

# Pack
npm pack