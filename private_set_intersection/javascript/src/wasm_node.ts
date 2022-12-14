import psi from 'psi_wasm_node'

import { Loader, createLoader } from './main/loader'
import { PSILibrary } from './implementation/psi'
import { PSI } from './main/psi'

const Loader = (): Promise<Loader> => createLoader(psi)

/**
 * Main export for the library
 */
export default async (): Promise<PSILibrary> => {
  const psi = await PSI(Loader)
  return psi({ client: true, server: true })
}
