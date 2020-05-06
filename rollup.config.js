// import { terser } from 'rollup-plugin-terser'

export default [
  {
    input: 'psi_cardinality/javascript/src/index_client_wasm.js',
    output: {
      file: 'psi_cardinality/javascript/dist/client/wasm/index.js',
      sourcemap: true,
      format: 'umd',
      name: 'psi.js'
      // plugins: [terser()]
    }
  },
  {
    input: 'psi_cardinality/javascript/src/index_client_js.js',
    output: {
      file: 'psi_cardinality/javascript/dist/client/js/index.js',
      sourcemap: true,
      format: 'umd',
      name: 'psi.js'
      // plugins: [terser()]
    }
  }
]
