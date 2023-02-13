import PSI from '../wasm_node'
import { ERROR_INSTANCE_DELETED } from '../implementation/constants'
import { PSILibrary } from '../implementation/psi'
import { Response, ServerSetup } from '../implementation/proto/psi_pb'

let psi: PSILibrary
beforeAll(async () => {
  psi = await PSI()
})

describe('PSI Client', () => {
  test('It should create from an existing key', async () => {
    const client1 = psi.client!.createWithNewKey()
    const key = client1.getPrivateKeyBytes()

    const client2 = psi.client!.createFromKey(key)

    expect(client2.getPrivateKeyBytes()).toEqual(key)
  })

  test('It should fail to create from an invalid key', async () => {
    const key = Uint8Array.from({ length: 32 })
    expect(() => psi.client!.createFromKey(key)).toThrow()
  })
  test('It should throw if deleted twice', async () => {
    const client = psi.client!.createWithNewKey()

    client.delete()

    expect(client.delete).toThrow(ERROR_INSTANCE_DELETED)
  })

  test('It should return the private key as a binary array', async () => {
    const client = psi.client!.createWithNewKey()
    const keyLength = 32
    const key = client.getPrivateKeyBytes()
    expect(key.length).toBe(keyLength)
  })

  test('It should fail return the private key as a binary array if deleted', async () => {
    const client = psi.client!.createWithNewKey()
    client.delete()
    expect(client.getPrivateKeyBytes.bind(client)).toThrow(
      ERROR_INSTANCE_DELETED
    )
  })

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
