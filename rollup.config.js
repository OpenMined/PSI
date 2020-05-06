import { terser } from 'rollup-plugin-terser'

export default [
  /////////////////
  // Client
  /////////////////
  {
    input: 'psi_cardinality/javascript/src/index_client_wasm.js',
    output: {
      file: 'psi_cardinality/javascript/dist/client/wasm/index.js',
      sourcemap: true,
      format: 'umd',
      name: 'PSICardinality',
      plugins: [terser()]
    }
  },
  {
    input: 'psi_cardinality/javascript/src/index_client_js.js',
    output: {
      file: 'psi_cardinality/javascript/dist/client/js/index.js',
      sourcemap: true,
      format: 'umd',
      name: 'PSICardinality',
      plugins: [terser()]
    }
  },
  /////////////////
  // Server
  /////////////////
  {
    input: 'psi_cardinality/javascript/src/index_server_wasm.js',
    output: {
      file: 'psi_cardinality/javascript/dist/server/wasm/index.js',
      sourcemap: true,
      format: 'umd',
      name: 'PSICardinality',
      plugins: [terser()]
    }
  },
  {
    input: 'psi_cardinality/javascript/src/index_server_js.js',
    output: {
      file: 'psi_cardinality/javascript/dist/server/js/index.js',
      sourcemap: true,
      format: 'umd',
      name: 'PSICardinality',
      plugins: [terser()]
    }
  },
  /////////////////
  // Combined
  /////////////////
  {
    input: 'psi_cardinality/javascript/src/index_combined_wasm.js',
    output: {
      file: 'psi_cardinality/javascript/dist/combined/wasm/index.js',
      sourcemap: true,
      format: 'umd',
      name: 'PSICardinality',
      plugins: [terser()]
    }
  },
  {
    input: 'psi_cardinality/javascript/src/index_combined_js.js',
    output: {
      file: 'psi_cardinality/javascript/dist/combined/js/index.js',
      sourcemap: true,
      format: 'umd',
      name: 'PSICardinality',
      plugins: [terser()]
    }
  }
]
