import clientWasmLibrary from 'psi_cardinality_client_wasm'

import { createLoader } from './loader'
import { PSICardinalityConstructor } from './implementation/psi_cardinality'
import { ClientWrapperConstructor } from './implementation/client'

/**
 * Main export for the library
 */
export default PSICardinalityConstructor({
  clientWrapper: ClientWrapperConstructor({
    loader: createLoader(clientWasmLibrary)
  })
})
