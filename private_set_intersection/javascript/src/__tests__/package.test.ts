import PSI from '../wasm_node'
import { PSILibrary } from '../implementation/psi'

let psi: PSILibrary
beforeAll(async () => {
  psi = await PSI()
})

describe('PSI Package', () => {
  test('It should return the version string', async () => {
    const { version } = psi.package
    expect(typeof version).toBe('string')
  })
})
