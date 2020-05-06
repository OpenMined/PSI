import benchmarkWasmLibrary from './bin/psi_cardinality_benchmark_wasm.js'
import benchmarkJsLibrary from './bin/psi_cardinality_benchmark_js.js'

import { Loader } from './src/loader'

/**
 * Detect which source to run, defaults to wasm
 */
const runJs = process.env.RUN_DEMO === 'js'

;(async () => {
  await Loader(runJs ? benchmarkJsLibrary : benchmarkWasmLibrary)()
})()
