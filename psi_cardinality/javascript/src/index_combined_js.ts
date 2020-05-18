import combinedJsLibrary from 'psi_cardinality_combined_js'

import { NestedLibrary, createLoader } from './loader'
import {
  PSICardinalityLibrary,
  PSICardinalityConstructor
} from './implementation/psi_cardinality'
import { PackageWrapperConstructor } from './implementation/package'
import { ClientWrapperConstructor } from './implementation/client'
import { ServerWrapperConstructor } from './implementation/server'

const Loader = (): Promise<NestedLibrary> => createLoader(combinedJsLibrary)

/**
 * Main export for the library
 */
export default async (): Promise<PSICardinalityLibrary> =>
  PSICardinalityConstructor({
    packageWrapper: PackageWrapperConstructor({
      loader: await Loader()
    }),
    serverWrapper: ServerWrapperConstructor({
      loader: await Loader()
    }),
    clientWrapper: ClientWrapperConstructor({
      loader: await Loader()
    })
  })
