import clientJsLibrary from 'psi_cardinality_client_js'

import { NestedLibrary, createLoader } from './loader'
import {
  PSICardinalityLibrary,
  PSICardinalityConstructor
} from './implementation/psi_cardinality'
import { PackageWrapperConstructor } from './implementation/package'
import { ClientWrapperConstructor } from './implementation/client'

const Loader = (): Promise<NestedLibrary> => createLoader(clientJsLibrary)

/**
 * Main export for the library
 */
export default async (): Promise<PSICardinalityLibrary> =>
  PSICardinalityConstructor({
    packageWrapper: PackageWrapperConstructor({
      loader: await Loader()
    }),
    clientWrapper: ClientWrapperConstructor({
      loader: await Loader()
    })
  })
