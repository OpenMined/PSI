import PSI from '../index_combined_wasm'
import { PSILibrary } from 'src/implementation/psi'

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
