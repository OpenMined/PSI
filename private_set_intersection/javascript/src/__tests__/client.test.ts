import { ERROR_INSTANCE_DELETED } from '../implementation/constants'
import { Response, ServerSetup } from '../implementation/proto/psi_pb'
import { PSILibrary } from '../implementation/psi'
import PSI from '../wasm_node'

let psi: PSILibrary
beforeAll(async () => {
  psi = await PSI()
})

describe('PSI Client', () => {
  test('It should create a request', async () => {
    const client = psi.client!.createWithNewKey()
    const numClientElements = 10
    const clientInputs = Array.from(
      { length: numClientElements },
      (_, i) => `Element ${i}`
    )

    const request = client.createRequest(clientInputs)
    expect(request.getEncryptedElementsList().length).toStrictEqual(
      numClientElements
    )
  })

  test('It should throw if attempting to create a request after deletion', async () => {
    const client = psi.client!.createWithNewKey()
    const numClientElements = 100
    const clientInputs = Array.from(
      { length: numClientElements },
      (_, i) => `Element ${i}`
    )

    client.delete()

    expect(client.createRequest.bind(client, clientInputs)).toThrow(
      ERROR_INSTANCE_DELETED
    )
  })

  test('It should fail to process a response (cardinality)', async () => {
    const client = psi.client!.createWithNewKey()
    const serverSetup = new ServerSetup()
    const serverResponse = new Response()

    expect(
      client.getIntersectionSize.bind(client, serverSetup, serverResponse)
    ).toThrow()
  })

  test('It should fail to process a response (intersection)', async () => {
    const client = psi.client!.createWithNewKey(true)
    const serverSetup = new ServerSetup()
    const serverResponse = new Response()

    expect(
      client.getIntersection.bind(client, serverSetup, serverResponse)
    ).toThrow()
  })
})
