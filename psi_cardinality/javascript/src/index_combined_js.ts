import serverJsLibrary from 'psi_cardinality_server_js'
import clientJsLibrary from 'psi_cardinality_client_js'

import { createLoader } from './loader'
import { PSICardinalityConstructor } from './implementation/psi_cardinality'
import { ServerWrapperConstructor } from './implementation/server'
import { ClientWrapperConstructor } from './implementation/client'

/**
 * Main export for the library
 */
export default PSICardinalityConstructor({
  serverWrapper: ServerWrapperConstructor({
    loader: createLoader(serverJsLibrary)
  }),
  clientWrapper: ClientWrapperConstructor({
    loader: createLoader(clientJsLibrary)
  })
})
