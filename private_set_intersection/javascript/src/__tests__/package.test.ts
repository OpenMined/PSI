import PSICardinality from '../index_combined_wasm'
import { PSICardinalityLibrary } from 'src/implementation/psi'

let PSI: PSICardinalityLibrary
beforeAll(async () => {
  PSI = await PSICardinality()
})

describe('PSI Package', () => {
  test('It should return the version string', async () => {
    const { version } = PSI.package
    expect(typeof version).toBe('string')
  })
})
