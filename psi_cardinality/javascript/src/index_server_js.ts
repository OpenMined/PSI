import serverJsLibrary from '../bin/psi_cardinality_server_js.js'

import { createLoader } from './loader'
import { PSICardinalityConstructor } from './implementation/psi_cardinality'
import { ServerWrapperConstructor } from './implementation/server'

/**
 * Main export for the library
 */
export default PSICardinalityConstructor({
  serverWrapper: ServerWrapperConstructor({
    loader: createLoader(serverJsLibrary)
  })
})
