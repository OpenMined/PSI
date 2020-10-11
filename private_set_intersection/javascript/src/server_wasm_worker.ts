import clientJsLibrary from 'psi_server_wasm_worker'

import { Loader, createLoader } from './main/loader'
import { PSILibrary } from './implementation/psi'
import { PSI } from './main/psi'

const Loader = (): Promise<Loader> => createLoader(clientJsLibrary)

/**
 * Main export for the library
 */
export default async (): Promise<PSILibrary> => {
  const psi = await PSI(Loader)
  return psi({ server: true })
}
