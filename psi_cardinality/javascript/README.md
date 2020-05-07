# PSI Cardinality - JavaScript

Private Set Intersection Cardinality protocol based on ECDH and Bloom Filters.

## Installing

To install, run:

```bash
npm install @openmined/psi.js
# or with yarn
yarn add @openmined/psi.js
```

Then import the package:

```javascript
import PSI from '@openmined/psi.js'

const server = await PSI.server.createWithNewKey()
const client = await PSI.client.create()
```

By default, the package will use the `combined` build with the `wasm` variant for getting started.
This includes both `client` and `server` implementations, but often only one is used. We offer deep import
links to only load what is needed for your specific environment.

The deep import structure is as follows:
`<package name>/<client|server|combined>/<wasm|js>/<umd|es>`

To only load the `client`:

```javascript
// Loads only the client, supporting WebAssembly or asm.js
// with either `umd` (Browser + NodeJS) or `es` (ES6 modules)
import PSI from '@openmined/psi.js/client/wasm/umd'
import PSI from '@openmined/psi.js/client/wasm/es'
import PSI from '@openmined/psi.js/client/js/umd'
import PSI from '@openmined/psi.js/client/js/es'

const client = await PSI.client.create()
// PSI.server is not implemented
```

To only load the `server`:

```javascript
// Loads only the server, supporting WebAssembly or asm.js
// with either `umd` (Browser + NodeJS) or `es` (ES6 modules)
import PSI from '@openmined/psi.js/server/wasm/umd'
import PSI from '@openmined/psi.js/server/wasm/es'
import PSI from '@openmined/psi.js/server/js/umd'
import PSI from '@openmined/psi.js/server/js/es'

const server = await PSI.server.createWithNewKey()
// PSI.client is not implemented
```

## Compiling and Running

### Requirements

Ensure your environment has the following global dependencies:

- [Bazel](https://bazel.build)
- [NodeJS](https://nodejs.org/en/)
- [Yarn](https://yarnpkg.com/)

Next, ensure you have updated submodules

```
yarn submodule:update
```

Then, update and initialize `emsdk`

```
yarn em:update
yarn em:init
```

Now, install the rest of the dev dependencies

```
yarn install
```

To build the client, server, or both for WebAssembly and pure JS

```
yarn build:client
yarn build:server

# or both
yarn build
```

Run the tests or generate coverage reports. **Note** tests are run using the WASM variant.

```
yarn test

# or to see coverage
yarn coverage
```

## Benchmarks

Build the benchmark for WebAssembly, pure JS, or both variants

```
yarn build:benchmark:wasm
yarn build:benchmark:js

# or both
yarn build:benchmark
```

Finally, run the benchmark for WebAssembly or pure JS

```
yarn benchmark:wasm
yarn benchmark:js
```

## Publishing

Ensure we start with a clean build

`yarn clean`

Build both client and server

`yarn build`

Compile typescript

`yarn compile`

Test your changes and check code coverage

```bash
# Test TS
yarn test

# Test compiled JS
yarn test:js

# Generate coverage report
yarn coverage
```

Then, bundle all the files

`yarn rollup`

Now, we can test publish

`yarn publish:test`

Finally, publish the bundle

`yarn run publish`

**Note**: The default `npm publish`/`yarn publish` has been disabled to prevent publishing of the entire project files.
Instead, we have a custom override which will publish the npm package from a specific directory.
This allows us to publish a single package with shortened deep import links that specify the
different targets listed above.
