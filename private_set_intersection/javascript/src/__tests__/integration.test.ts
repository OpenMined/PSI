import { PSILibrary } from '../implementation/psi'
import PSI from '../wasm_node'

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

  test('It should create from a static key', async () => {
    const server = psi.server!.createFromKey(serverKey, false)
    expect(server.getPrivateKeyBytes()).toEqual(serverKey)
    const client = psi.client!.createFromKey(clientKey, false)
    expect(client.getPrivateKeyBytes()).toEqual(clientKey)
  })

  test('should compute the intersection', async () => {
    ;[
      { revealIntersection: true, dataStructure: psi.dataStructure.Raw },
      { revealIntersection: true, dataStructure: psi.dataStructure.GCS },
      {
        revealIntersection: true,
        dataStructure: psi.dataStructure.BloomFilter
      },
      { revealIntersection: false, dataStructure: psi.dataStructure.Raw },
      { revealIntersection: false, dataStructure: psi.dataStructure.GCS },
      {
        revealIntersection: false,
        dataStructure: psi.dataStructure.BloomFilter
      }
    ].forEach(({ revealIntersection, dataStructure }) => {
      const client = psi.client!.createWithNewKey(revealIntersection)
      const server = psi.server!.createWithNewKey(revealIntersection)

      const serverSetup = server
        .createSetupMessage(fpr, numClientElements, serverInputs, dataStructure)
        .serializeBinary()

      const clientRequest = client.createRequest(clientInputs).serializeBinary()
      const serverResponse = server
        .processRequest(psi.request.deserializeBinary(clientRequest))
        .serializeBinary()

      if (revealIntersection === true) {
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
          // Test that the intersection is within 10% error margin to match `fpr`
          // eslint-disable-next-line jest/no-conditional-expect
          expect(intersection.length).toBeGreaterThanOrEqual(
            numClientElements / 2
          )
          // eslint-disable-next-line jest/no-conditional-expect
          expect(intersection.length).toBeLessThanOrEqual(
            (numClientElements / 2) * 1.1
          )
        }
      } else {
        const intersection = client.getIntersectionSize(
          psi.serverSetup.deserializeBinary(serverSetup),
          psi.response.deserializeBinary(serverResponse)
        )
        // Test that the intersection is within 10% error margin to match `fpr`
        // eslint-disable-next-line jest/no-conditional-expect
        expect(intersection).toBeGreaterThanOrEqual(
          Math.floor(numClientElements / 2)
        )
        // eslint-disable-next-line jest/no-conditional-expect
        expect(intersection).toBeLessThanOrEqual(
          Math.ceil((numClientElements / 2) * 1.1)
        )
      }
    })
  })
})
