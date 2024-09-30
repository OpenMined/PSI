![om-logo](https://github.com/OpenMined/design-assets/blob/master/logos/OM/horizontal-primary-trans.png)

[![CD](https://github.com/OpenMined/PSI/actions/workflows/CD.yml/badge.svg?event=release)](https://github.com/OpenMined/PSI/actions/workflows/CD.yml)
![License](https://img.shields.io/github/license/OpenMined/PSI)
![OpenCollective](https://img.shields.io/opencollective/all/openmined)

# PSI - JavaScript

Private Set Intersection protocol based on ECDH, Bloom Filters, and Golomb
Compressed Sets. The goal of this library is to allow a server to compute and
return the intersection or intersection size (cardinality) from a set on the
server and a set from a client without the server learning anything about the
client's set.

- 💾 Low memory footprint
- 🚀 Fastest implementation using WebAssembly
- 🔥 Works in any client / server configuration, even [React
  Native](https://reactnative.dev/)!
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

By **default**, the package will use the `psi` build with the `wasm` targeting
the `node` environment.

The deep import structure is as follows: `<package name> /
<psi>_<wasm>_<node|web|worker>`

Example:

```javascript
import PSI from '@openmined/psi.js/psi_wasm_node.js'
import PSI from '@openmined/psi.js/psi_wasm_web.js'
import PSI from '@openmined/psi.js/psi_wasm_worker.js'
```

## React-Native

The bundle needs a bit of extra work. Specifically, it expects the browser
`crypto.getRandomValues` which it will not find by default as react-native
doesn't support the crypto builtin. It can be fixed by `npm install
react-native-get-random-values` which provides access to this global while
supporting a CSPRNG. The library also needs to have the browser `document` which
is an artifact from the build system. Simply provide `global.document = {}`.
Finally, it requires the following deep import structure:

```javascript
// Provide a CSPRNG mapping to crypto.getRandomValues()
import 'react-native-get-random-values'
import PSI from '@openmined/psi.js/psi_wasm_web'
;(async () => {
  // Spoof the browser document
  global.document = {}
  // Wait for the library to initialize
  const psi = await PSI()

  const client = psi.client.createWithNewKey()
  const server = psi.server.createWithNewKey()
  //...
})()
```

## Example

We show an example following the steps:

1. Initialize the server and create the `server setup` to be sent later
2. Initialize the client and create a client `request` to be sent to the server
3. Process the `request` and send both the `server setup` and `response` to the
   client
4. Client receives the `setup` and `response` and computes the intersection

```javascript
const PSI = require('@openmined/psi.js')

;(async () => {
  const psi = await PSI()

  // Define mutually agreed upon parameters for both client and server
  const fpr = 0.001 // false positive rate (0.1%)
  const numClientElements = 10 // Size of the client set to check
  const numTotalElements = 100 // Maximum size of the server set
  const revealIntersection = false // Allows to reveal the intersection (true)
  /******************
   *** 1. Server ****
   ******************/
  // Create new server instance
  //
  // By default, the server will only support calculating
  // the intersection size (cardinality). To obtain the
  // intersection of the two sets, pass in the boolean `true`.
  //
  // Ex: const server = psi.server.createWithNewKey(true)
  const server = psi.server.createWithNewKey(revealIntersection)
  // Specifying no parameters is equivalent to passing in `false`
  // const server = psi.server.createWithNewKey()

  // Example server set of data
  const serverInputs = Array.from(
    { length: numTotalElements },
    (_, i) => `Element ${i * 2}`
  )

  // Create the setup message that will later
  // be used to compute the intersection. By default,
  // this function will use Golomb Compressed Sets (GCS).
  // You may pass in an extra argument at the end to pick
  // the bloom filter option.
  const serverSetup = server.createSetupMessage(
    fpr,
    numClientElements,
    serverInputs
    // psi.dataStructure.GCS // This is the default and can omitted
  )

  // Example with regular bloom filter
  // const serverSetup = server.createSetupMessage(
  //   fpr,
  //   numClientElements,
  //   serverInputs,
  //   psi.dataStructure.BloomFilter
  // )

  /******************
   *** 2. Client ****
   ******************/
  // Create new client instance
  //
  // By default, the client will only support calculating
  // the intersection size (cardinality). To obtain the
  // intersection of the two sets, pass in the boolean `true`.
  //
  // Ex: const server = psi.client.createWithNewKey(true)
  const client = psi.client.createWithNewKey(revealIntersection)
  // Specifying no parameters is equivalent to passing in `false`
  // const client = psi.client.createWithNewKey()

  // Example client set of data to check
  const clientInputs = Array.from(
    { length: numClientElements },
    (_, i) => `Element ${i}`
  )
  // Create a client request to send to the server
  const clientRequest = client.createRequest(clientInputs)

  // Serialize the client request. Will be a Uint8Array.
  const serializedClientRequest = clientRequest.serializeBinary()

  // ... send the serialized client request from client -> server

  /******************
   *** 3. Server ****
   ******************/
  // Deserialize the client request for the server
  const deserializedClientRequest = psi.request.deserializeBinary(
    serializedClientRequest
  )

  // Process the client's request and return to the client
  const serverResponse = server.processRequest(deserializedClientRequest)

  // Serialize the server response. Will be a Uint8Array.
  const serializedServerResponse = serverResponse.serializeBinary()

  // Serialize the server setup. Will be an Uint8Array.
  const serializedServerSetup = serverSetup.serializeBinary()
  // ... send the serialized server setup _and_ server response from server -> client

  /******************
   *** 4. Client ****
   ******************/
  // Deserialize the server response
  const deserializedServerResponse = psi.response.deserializeBinary(
    serializedServerResponse
  )

  // Deserialize the server setup
  const deserializedServerSetup = psi.serverSetup.deserializeBinary(
    serializedServerSetup
  )

  // NOTE:
  // A client can always compute the intersection size (cardinality), but by
  // default does not reveal the actual intersection between the two arrays.
  // This is dependent on whether or not _both_ client/server were initialized
  // with the same boolean `true` value which sets an internal `reveal_intersection` flag.
  // Any calls made to `getIntersection` will throw an error if this flag was not set to true.

  // Reveal the cardinality
  const intersectionSize = client.getIntersectionSize(
    deserializedServerSetup,
    deserializedServerResponse
  )
  console.log('intersectionSize', intersectionSize)
  // intsersectionSize 5

  // Reveal the intersection (only if `revealIntersection` was set to true)
  // const intersection = client.getIntersection(
  //   deserializedServerSetup,
  //   deserializedServerResponse
  // )
  // console.log('intersection', intersection)
  // intersection [ 0, 2, 4, 6, 8 ]
})()
```

## Contributors

See
[CONTRIBUTORS.md](https://github.com/OpenMined/PSI/blob/master/CONTRIBUTORS.md).

## Contributing

Pull requests are welcome. For major changes, please open an issue first to
discuss what you would like to change.

Please make sure to update tests as appropriate.

### Requirements

Ensure your environment has the following global dependencies:

- [Bazel](https://bazel.build)
- [NodeJS](https://nodejs.org/en/)

Now, install the rest of the dev dependencies

```bash
npm install
```

To compile the client, server, or psi (both client and server) for WebAssembly
and pure JS

```bash
npm run build
```

Now, build the JS protobufs for TypeScript.

```bash
npm run build:proto
```

Compile TypeScript to JS (sanity checking)

```bash
npm run compile
```

Run the tests or generate coverage reports. **Note** tests are run using the
WASM variant.

```bash
npm run test
npm run coverage
```

### Benchmarks

Build the benchmark for WebAssembly and pure JS

```bash
npm run compile
npm run build:benchmark
```

Finally, run the benchmark for WebAssembly or pure JS

```bash
npm run benchmark:wasm
```

### Publishing

Ensure we start with a clean build

`npm run clean`

Build the client, server, and psi (client and server)

`npm run build`

Build protobufs

`npm run build:proto`

Compile typescript (sanity check)

`npm run compile`

Test your changes and check code coverage

```bash
# Test TS
npm run test

# Generate coverage report
npm run coverage
```

Then, bundle all the files

`npm run rollup`

Now, we can test publish

`npm run publish:test`

Finally, publish the bundle

`npm run publish`

**Note**: The default `npm publish` has been disabled to prevent publishing of
the entire project files. Instead, we have a custom override which will publish
the npm package from a specific directory. This allows us to publish a single
package with shortened deep import links that specify the different targets
listed above.

## Changes

See `JS` [CHANGES.md](https://github.com/OpenMined/PSI/blob/master/CHANGES.md).

## License

[Apache License 2.0](https://choosealicense.com/licenses/apache-2.0/)
