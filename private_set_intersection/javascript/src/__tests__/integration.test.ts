import PSI from '../wasm_node'
import { PSILibrary } from '../implementation/psi'

const clientKey = Uint8Array.from([
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
  22, 23, 24, 25, 26, 27, 28, 29, 30, 31
])
const serverKey = Uint8Array.from([
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
  23, 24, 25, 26, 27, 28, 29, 30, 31, 32
])
const fpr = 0.01
const numClientElements = 10
const numServerElements = 100
const clientInputs = Array.from(
  { length: numClientElements },
  (_, i) => `Element ${i}`
)
const serverInputs = Array.from(
  { length: numServerElements },
  (_, i) => `Element ${i * 2}`
)
let psi: PSILibrary

beforeAll(async () => {
  psi = await PSI()
})

describe('PSI Integration', () => {
  test('should return a version string', async () => {
    expect(typeof psi.package.version).toBe('string')
  })

  test('should return the intersection', async () => {
    ;[
      { dataStructure: psi.dataStructure.Raw },
      { dataStructure: psi.dataStructure.GCS },
      {
        dataStructure: psi.dataStructure.BloomFilter
      }
    ].forEach(({ dataStructure }) => {
      const client = psi.client!.createFromKey(clientKey, true)
      const server = psi.server!.createFromKey(serverKey, true)

      const serverSetup = server
        .createSetupMessage(fpr, numClientElements, serverInputs, dataStructure)
        .serializeBinary()

      const clientRequest = client.createRequest(clientInputs).serializeBinary()
      const serverResponse = server
        .processRequest(psi.request.deserializeBinary(clientRequest))
        .serializeBinary()

      const intersection = client.getIntersection(
        psi.serverSetup.deserializeBinary(serverSetup),
        psi.response.deserializeBinary(serverResponse)
      )

      const iset = new Set(intersection)
      for (let idx = 0; idx < numClientElements; idx++) {
        if (idx % 2 === 0) {
          // eslint-disable-next-line jest/no-conditional-expect
          expect(iset.has(idx)).toBeTruthy()
        } else {
          // eslint-disable-next-line jest/no-conditional-expect
          expect(iset.has(idx)).toBeFalsy()
        }
      }

      // Test that the intersection is within 10% error margin to match `fpr`
      expect(intersection.length).toBeGreaterThanOrEqual(numClientElements / 2)
      expect(intersection.length).toBeLessThan((numClientElements / 2) * 1.1)
    })
  })
  test('should return the intersection size (cardinality)', async () => {
    ;[
      { dataStructure: psi.dataStructure.Raw },
      { dataStructure: psi.dataStructure.GCS },
      {
        dataStructure: psi.dataStructure.BloomFilter
      }
    ].forEach(({ dataStructure }) => {
      const client = psi.client!.createFromKey(clientKey, false)
      const server = psi.server!.createFromKey(serverKey, false)

      const serverSetup = server
        .createSetupMessage(fpr, numClientElements, serverInputs, dataStructure)
        .serializeBinary()

      const clientRequest = client.createRequest(clientInputs).serializeBinary()
      const serverResponse = server
        .processRequest(psi.request.deserializeBinary(clientRequest))
        .serializeBinary()

      const intersection = client.getIntersectionSize(
        psi.serverSetup.deserializeBinary(serverSetup),
        psi.response.deserializeBinary(serverResponse)
      )
      // Test that the intersection is within 10% error margin to match `fpr`
      expect(intersection).toBeGreaterThanOrEqual(numClientElements / 2)
      expect(intersection).toBeLessThan((numClientElements / 2) * 1.1)
    })
  })
})
