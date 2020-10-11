/* eslint-env node */
import benchmarkWasmLibrary from './bin/psi_benchmark_wasm.js'
import benchmarkJsLibrary from './bin/psi_benchmark_js.js'

import { createLoader } from './tsc-out/main/loader'

/**
 * Detect which source to run, defaults to wasm
 */
const runJs = process.env.RUN_DEMO === 'js'

;(async () => {
  await createLoader(runJs ? benchmarkJsLibrary : benchmarkWasmLibrary)
})()
