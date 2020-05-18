import PSICardinality from '../index_combined_wasm'

describe('PSI Package', () => {
  test('It should return the version string', async () => {
    const PSI = await PSICardinality()
    const version = PSI.package.version
    expect(typeof version).toBe('string')
  })
})
