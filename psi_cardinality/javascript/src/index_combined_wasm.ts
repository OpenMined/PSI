import combinedWasmLibrary from 'psi_cardinality_combined_wasm'

import { createLoader } from './loader'
import { PSICardinalityConstructor } from './implementation/psi_cardinality'
import { ServerWrapperConstructor } from './implementation/server'
import { ClientWrapperConstructor } from './implementation/client'

const Loader = createLoader(combinedWasmLibrary)

/**
 * Main export for the library
 */
export default PSICardinalityConstructor({
  serverWrapper: ServerWrapperConstructor({
    loader: Loader
  }),
  clientWrapper: ClientWrapperConstructor({
    loader: Loader
  })
})
