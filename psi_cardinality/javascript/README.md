# PSI Cardinality - JavaScript

Private Set Intersection Cardinality protocol based on ECDH and Bloom Filters.

## Installing

To install, run:

```bash
npm install @openmined/psi-cardinality
# or with yarn
yarn add @openmined/psi-cardinality
```

Then import the package:

```javascript
import PSI from '@openmined/psi-cardinality'

const server = await PSI.Server.CreateWithNewKey()
const client = await PSI.Client.Create()
```

By default, the package will use the `combined` build with the `wasm` variant for getting started.
This includes both `Client` and `Server` implementations, but often only one is used. We offer deep import
links to only load what is needed for your specific environment.

The deep import structure is as follows:
`<package name>/<client|server|combined>/<wasm|js>/<umd|es>`

To only load the `Client`:

```javascript
// Loads only the client, supporting WebAssembly or asm.js
// with either `umd` (Browser + NodeJS) or `es` (ES6 modules)
import PSI from '@openmined/psi-cardinality/client/wasm/umd'
import PSI from '@openmined/psi-cardinality/client/wasm/es'
import PSI from '@openmined/psi-cardinality/client/js/umd'
import PSI from '@openmined/psi-cardinality/client/js/es'

const client = await PSI.Client.Create()
// PSI.Server is not implemented
```

To only load the `Server`:

```javascript
// Loads only the server, supporting WebAssembly or asm.js
// with either `umd` (Browser + NodeJS) or `es` (ES6 modules)
import PSI from '@openmined/psi-cardinality/server/wasm/umd'
import PSI from '@openmined/psi-cardinality/server/wasm/es'
import PSI from '@openmined/psi-cardinality/server/js/umd'
import PSI from '@openmined/psi-cardinality/server/js/es'

const server = await PSI.Server.CreateWithNewKey()
// PSI.Client is not implemented
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

Test your changes and check code coverage

`yarn coverage`

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
