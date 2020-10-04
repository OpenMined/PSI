import combinedJsLibrary from 'psi_combined_js_worker'

import { NestedLibrary, createLoader } from './loader'
import { PSILibrary, PSIConstructor } from './implementation/psi'
import { PackageWrapperConstructor } from './implementation/package'
import { ClientWrapperConstructor } from './implementation/client'
import { ServerWrapperConstructor } from './implementation/server'

const Loader = (): Promise<NestedLibrary> => createLoader(combinedJsLibrary)

/**
 * Main export for the library
 */
export default async (): Promise<PSILibrary> =>
  PSIConstructor({
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
