import combinedJsLibrary from 'psi_cardinality_combined_js'

import { createLoader } from './loader'
import { PSICardinalityConstructor } from './implementation/psi_cardinality'
import { ServerWrapperConstructor } from './implementation/server'
import { ClientWrapperConstructor } from './implementation/client'

const Loader = createLoader(combinedJsLibrary)

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
