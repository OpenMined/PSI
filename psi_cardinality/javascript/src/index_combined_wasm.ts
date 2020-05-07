import serverWasmLibrary from 'psi_cardinality_server_wasm'
import clientWasmLibrary from 'psi_cardinality_client_wasm'

import { createLoader } from './loader'
import { PSICardinalityConstructor } from './implementation/psi_cardinality'
import { ServerWrapperConstructor } from './implementation/server'
import { ClientWrapperConstructor } from './implementation/client'

/**
 * Main export for the library
 */
export default PSICardinalityConstructor({
  serverWrapper: ServerWrapperConstructor({
    loader: createLoader(serverWasmLibrary)
  }),
  clientWrapper: ClientWrapperConstructor({
    loader: createLoader(clientWasmLibrary)
  })
})
