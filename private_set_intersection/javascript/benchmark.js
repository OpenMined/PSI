/* eslint-env node */
import benchmark from './bin/psi_benchmark_wasm.js'

import { createLoader } from './tsc-out/main/loader'
;(async () => {
  await createLoader(benchmark)
})()
