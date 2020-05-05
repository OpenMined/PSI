import clientWasmLibrary from '../bin/psi_cardinality_client_wasm.js'

import { Loader } from './loader'
import { PSICardinalityImpl } from './implementation/psi_cardinality'
import { ClientImpl } from './implementation/client'

/**
 * Main export for the library
 */
export default PSICardinalityImpl({
  ClientImpl: ClientImpl({
    Loader: Loader(clientWasmLibrary)
  })
})
