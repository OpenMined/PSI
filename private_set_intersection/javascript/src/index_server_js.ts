import serverJsLibrary from 'psi_server_js'

import { NestedLibrary, createLoader } from './loader'
import {
  PSICardinalityLibrary,
  PSICardinalityConstructor
} from './implementation/psi'
import { PackageWrapperConstructor } from './implementation/package'
import { ServerWrapperConstructor } from './implementation/server'

const Loader = (): Promise<NestedLibrary> => createLoader(serverJsLibrary)

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
    })
  })
