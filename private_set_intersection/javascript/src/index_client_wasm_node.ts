import clientWasmLibrary from 'psi_client_wasm_node'

import { NestedLibrary, createLoader } from './loader'
import { PSILibrary, PSIConstructor } from './implementation/psi'
import { PackageWrapperConstructor } from './implementation/package'
import { ClientWrapperConstructor } from './implementation/client'

const Loader = (): Promise<NestedLibrary> => createLoader(clientWasmLibrary)

/**
 * Main export for the library
 */
export default async (): Promise<PSILibrary> =>
  PSIConstructor({
    packageWrapper: PackageWrapperConstructor({
      loader: await Loader()
    }),
    clientWrapper: ClientWrapperConstructor({
      loader: await Loader()
    })
  })
