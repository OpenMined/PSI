import serverJsLibrary from '../bin/psi_cardinality_server_js.js'

import { Loader } from './loader'
import { PSICardinalityImpl } from './implementation/psi_cardinality'
import { ServerImpl } from './implementation/server'

/**
 * Main export for the library
 */
export default PSICardinalityImpl({
  ServerImpl: ServerImpl({
    Loader: Loader(serverJsLibrary)
  })
})
