// import { terser } from 'rollup-plugin-terser'

export default {
  input: 'psi_cardinality/javascript/src/index.js',
  output: {
    file: 'psi_cardinality/javascript/dist/psi.js',
    format: 'cjs',
    name: 'psi.js'

    // plugins: [terser()]
  }
}
