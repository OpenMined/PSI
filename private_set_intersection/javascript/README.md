![om-logo](https://github.com/OpenMined/design-assets/blob/master/logos/OM/horizontal-primary-trans.png)

[![Tests](https://github.com/OpenMined/PSI/workflows/Tests/badge.svg?branch=master&event=push)](https://github.com/OpenMined/PSI/actions?query=workflow%3ATests+branch%3Amaster+event%3Apush)
![License](https://img.shields.io/github/license/OpenMined/PSI)
![OpenCollective](https://img.shields.io/opencollective/all/openmined)

# PSI - JavaScript

Private Set Intersection protocol based on ECDH and Bloom Filters. The goal of this library is to allow a server to compute and return the intersection or intersection size (cardinality) from a set on the server and a set from a client without the server learning anything about the client's set.

- 💾 Low memory footprint
- 🚀 Fastest implementation using WebAssembly
- 🔥 Works in any client / server configuration
- 😎 Privacy preserving

## Installation

To install, run:

```bash
npm install @openmined/psi.js
# or with yarn
yarn add @openmined/psi.js
```

## Usage

Then `import` or `require` the package:

```javascript
import PSI from '@openmined/psi.js'
// or
const PSI = require('@openmined/psi.js')
```

By **default**, the package will use the `combined` build with the `wasm` target using the `umd` variant. This includes both `client` and `server` implementations, but often only one is used. We offer deep import
links to only load what is needed for your specific environment.

The deep import structure is as follows:
`<package name> / <client|server|combined> / <wasm|js> / <umd|es>`

To only load the `client`:

```javascript
// Loads only the client, supporting WebAssembly or asm.js
// with either `umd` (Browser + NodeJS) or `es` (ES6 modules)
// Pick one of the following:
import PSI from '@openmined/psi.js/client/wasm/umd'
import PSI from '@openmined/psi.js/client/wasm/es'
import PSI from '@openmined/psi.js/client/js/umd'
import PSI from '@openmined/psi.js/client/js/es'
;(async () => {
  // Wait for the library to initialize
  const psi = await PSI()

  const client = psi.client.createWithNewKey()
  // psi.server is not implemented
})()
```

To only load the `server`:

```javascript
// Loads only the server, supporting WebAssembly or asm.js
// with either `umd` (Browser + NodeJS) or `es` (ES6 modules)
// Pick one of the following:
import PSI from '@openmined/psi.js/server/wasm/umd'
import PSI from '@openmined/psi.js/server/wasm/es'
import PSI from '@openmined/psi.js/server/js/umd'
import PSI from '@openmined/psi.js/server/js/es'
;(async () => {
  // Wait for the library to initialize
  const psi = await PSI()

  const server = psi.server.createWithNewKey()
  // psi.client is not implemented
})()
```

To **manually** override the `combined` default import:

```javascript
// Loads the combined server and client, supporting WebAssembly or asm.js
// with either `umd` (Browser + NodeJS) or `es` (ES6 modules)
// Pick one of the following:
import PSI from '@openmined/psi.js/combined/wasm/umd' // Default
import PSI from '@openmined/psi.js/combined/wasm/es'
import PSI from '@openmined/psi.js/combined/js/umd'
import PSI from '@openmined/psi.js/combined/js/es'
;(async () => {
  // Wait for the library to initialize
  const psi = await PSI()

  const client = psi.client.createWithNewKey()
  const server = psi.server.createWithNewKey()
})()
```

## Example (intersection size)

```javascript
const PSI = require('@openmined/psi.js')

;(async () => {
  const psi = await PSI()

  // Create new server and client instances
  const server = psi.server.createWithNewKey()
  const client = psi.client.createWithNewKey()

  // Define mutually agreed upon parameters
  const fpr = 0.001 // false positive rate (0.1%)
  const numClientElements = 10 // Size of the client set to check
  const numTotalElements = 100 // Maximum size of the server set

  // Example server set of data
  const serverInputs = Array.from(
    { length: numTotalElements },
    (_, i) => `Element ${i * 2}`
  )

  // Example client set of data to check
  const clientInputs = Array.from(
    { length: numClientElements },
    (_, i) => `Element ${i}`
  )

  // Create the setup message that will later
  // be used to compute the intersection. Send to client
  const serverSetup = server.createSetupMessage(
    fpr,
    numClientElements,
    serverInputs
  )

  // Create a client request to send to the server
  const clientRequest = client.createRequest(clientInputs)

  // Process the client's request and return to the client
  const serverResponse = server.processRequest(clientRequest)

  // Client computes the intersection cardinality
  // and the server has learned nothing!
  const intersection = client.getIntersectionSize(serverSetup, serverResponse)
  // intersection = 5
})()
```

## Example (intersection)

```javascript
const PSI = require('@openmined/psi.js')

;(async () => {
  const psi = await PSI()

  // Create new server and client instances
  const server = psi.server.createWithNewKey(true)
  const client = psi.client.createWithNewKey(true)

  // Define mutually agreed upon parameters
  const fpr = 0.001 // false positive rate (0.1%)
  const numClientElements = 10 // Size of the client set to check
  const numTotalElements = 100 // Maximum size of the server set

  // Example server set of data
  const serverInputs = Array.from(
    { length: numTotalElements },
    (_, i) => `Element ${i * 2}`
  )

  // Example client set of data to check
  const clientInputs = Array.from(
    { length: numClientElements },
    (_, i) => `Element ${i}`
  )

  // Create the setup message that will later
  // be used to compute the intersection. Send to client
  const serverSetup = server.createSetupMessage(
    fpr,
    numClientElements,
    serverInputs
  )

  // Create a client request to send to the server
  const clientRequest = client.createRequest(clientInputs)

  // Process the client's request and return to the client
  const serverResponse = server.processRequest(clientRequest)

  // Client computes the intersection
  // and the server has learned nothing!
  const intersection = client.getIntersection(serverSetup, serverResponse)
  // intersection [ 0, 2, 4, 6, 8 ]
})()
```

## Contributors

See [CONTRIBUTORS.md](CONTRIBUTORS.md).

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

### Requirements

Ensure your environment has the following global dependencies:

- [Bazel](https://bazel.build)
- [NodeJS](https://nodejs.org/en/)

Next, ensure you have updated submodules

```
npm run submodule:update
```

Then, update and initialize `emsdk`

```
npm run em:update
npm run em:init
```

Now, install the rest of the dev dependencies

```
npm install
```

To build the client, server, or combined (both client and server) for WebAssembly and pure JS

```
npm run build
```

Run the tests or generate coverage reports. **Note** tests are run using the WASM variant.

```
npm run test

# or to test and see coverage
npm run coverage
```

### Benchmarks

Build the benchmark for WebAssembly and pure JS

```
npm run build:benchmark
```

Finally, run the benchmark for WebAssembly or pure JS

```
npm run benchmark:wasm
npm run benchmark:js
```

### Publishing

Ensure we start with a clean build

`npm run clean`

Build the client, server, and combined (client and server)

`npm run build`

Compile typescript

`npm run compile`

Test your changes and check code coverage

```bash
# Test TS
npm run test

# Test compiled JS
npm run test:js

# Generate coverage report
npm run coverage
```

Then, bundle all the files

`npm run rollup`

Now, we can test publish

`npm run publish:test`

Finally, publish the bundle

`npm run publish`

**Note**: The default `npm publish` has been disabled to prevent publishing of the entire project files.
Instead, we have a custom override which will publish the npm package from a specific directory.
This allows us to publish a single package with shortened deep import links that specify the
different targets listed above.

## License

[Apache License 2.0](https://choosealicense.com/licenses/apache-2.0/)
