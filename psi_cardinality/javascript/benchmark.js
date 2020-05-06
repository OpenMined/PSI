import benchmarkWasmLibrary from './bin/psi_cardinality_benchmark_wasm.js'
import benchmarkJsLibrary from './bin/psi_cardinality_benchmark_js.js'

import { Loader } from './src/loader'

/**
 * Detect which source to run, defaults to wasm
 */
const runWasm = process.env.RUN_DEMO
  ? Boolean(process.env.RUN_DEMO === 'wasm')
  : true

;(async () => {
  await Loader(runWasm ? benchmarkWasmLibrary : benchmarkJsLibrary)()
})()
