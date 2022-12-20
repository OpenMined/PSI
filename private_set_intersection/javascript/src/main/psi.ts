import { PSILibrary, PSIConstructor } from '../implementation/psi'
import { PackageWrapperConstructor } from '../implementation/package'
import { DataStructureWrapperConstructor } from '../implementation/dataStructure'
import { ClientWrapperConstructor } from '../implementation/client'
import { ServerWrapperConstructor } from '../implementation/server'
import { Loader } from './loader'

type InitOpts = {
  client?: boolean
  server?: boolean
}

type Init = (opts: InitOpts) => PSILibrary

/**
 * Main export for the library
 */
export const PSI = async (Loader: () => Promise<Loader>): Promise<Init> => {
  const loader = await Loader()
  return ({ client, server }): PSILibrary => {
    // Initialize dependencies
    const dataStructure = DataStructureWrapperConstructor({
      loader
    })

    return PSIConstructor({
      packageWrapper: PackageWrapperConstructor({
        loader
      }),
      dataStructureWrapper: dataStructure,
      ...(client && {
        clientWrapper: ClientWrapperConstructor({
          loader
        })
      }),
      ...(server && {
        serverWrapper: ServerWrapperConstructor({
          loader
        })({ DataStructure: dataStructure })
      })
    })
  }
}
