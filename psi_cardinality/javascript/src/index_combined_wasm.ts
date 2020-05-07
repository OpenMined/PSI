import serverWasmLibrary from '../bin/psi_cardinality_server_wasm.js'
import clientWasmLibrary from '../bin/psi_cardinality_client_wasm.js'

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
