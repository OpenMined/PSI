import serverWasmLibrary from 'psi_server_wasm_node'

import { NestedLibrary, createLoader } from './loader'
import { PSILibrary, PSIConstructor } from './implementation/psi'
import { PackageWrapperConstructor } from './implementation/package'
import { ServerWrapperConstructor } from './implementation/server'

const Loader = (): Promise<NestedLibrary> => createLoader(serverWasmLibrary)

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
    })
  })
