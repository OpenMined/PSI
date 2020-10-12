import PSI from '../combined_wasm_node'
import { PSILibrary } from 'src/implementation/psi'

let psi: PSILibrary
beforeAll(async () => {
  psi = await PSI()
})

describe('PSI Integration', () => {
  test('It should return the intersection', async () => {
    const client = psi.client!.createWithNewKey(true)
    const key = Uint8Array.from(
      // eslint-disable-next-line no-undef
      Buffer.from('djY2Bgt4JwfbjvJn6dDzpwrTvKWVE1Ks458mlrd1/tY=', 'base64')
    )
    const server = psi.server!.createFromKey(key, true)
    const fpr = 0.01
    const numClientElements = 100
    const numServerElements = 1000
    const clientInputs = Array.from(
      { length: numClientElements },
      (_, i) => `Element ${i}`
    )
    const serverInputs = Array.from(
      { length: numServerElements },
      (_, i) => `Element ${i * 2}`
    )
    const serverSetup = server.createSetupMessage(
      fpr,
      numClientElements,
      serverInputs
    )
    const clientRequest = client.createRequest(clientInputs)
    const serverResponse = server.processRequest(clientRequest)
    const intersection = client.getIntersection(serverSetup, serverResponse)
    // Test that the intersection is within 10% error margin to match `fpr`
    expect(intersection.length).toBeGreaterThanOrEqual(numClientElements / 2)
    expect(intersection.length).toBeLessThan((numClientElements / 2) * 1.1)
  })
  test('It should return the intersection size', async () => {
    const client = psi.client!.createWithNewKey()
    const key = Uint8Array.from(
      // eslint-disable-next-line no-undef
      Buffer.from('djY2Bgt4JwfbjvJn6dDzpwrTvKWVE1Ks458mlrd1/tY=', 'base64')
    )
    const server = psi.server!.createFromKey(key)
    const fpr = 0.01
    const numClientElements = 100
    const numServerElements = 1000
    const clientInputs = Array.from(
      { length: numClientElements },
      (_, i) => `Element ${i}`
    )
    const serverInputs = Array.from(
      { length: numServerElements },
      (_, i) => `Element ${i * 2}`
    )
    const serverSetup = server.createSetupMessage(
      fpr,
      numClientElements,
      serverInputs
    )
    const clientRequest = client.createRequest(clientInputs)
    const serverResponse = server.processRequest(clientRequest)
    const intersection = client.getIntersectionSize(serverSetup, serverResponse)
    // Test that the intersection is within 10% error margin to match `fpr`
    expect(intersection).toBeGreaterThanOrEqual(numClientElements / 2)
    expect(intersection).toBeLessThan((numClientElements / 2) * 1.1)
  })
})
