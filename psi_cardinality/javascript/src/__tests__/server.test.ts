import PSICardinality from '../index_combined_wasm'
import { ERROR_INSTANCE_DELETED } from '../implementation/constants'

describe('PSI Server', () => {
  test('It should create from an existing key', async () => {
    const PSI = await PSICardinality()
    const server1 = PSI.server!.createWithNewKey()
    const key = server1.getPrivateKeyBytes()

    const server2 = PSI.server!.createFromKey(key)

    expect(server2.getPrivateKeyBytes()).toEqual(key)
  })

  test('It should fail to create from an invalid key', async () => {
    const PSI = await PSICardinality()
    const key = Uint8Array.from({ length: 32 })
    expect(() => PSI.server!.createFromKey(key)).toThrow()
  })

  test('It should throw if deleted twice', async () => {
    const PSI = await PSICardinality()
    const server = PSI.server!.createWithNewKey()
    server.delete()
    expect(server.delete).toThrowError(ERROR_INSTANCE_DELETED)
  })

  test('It should return the private key as a binary array', async () => {
    const PSI = await PSICardinality()
    const server = PSI.server!.createWithNewKey()
    const keyLength = 32
    const key = server.getPrivateKeyBytes()
    expect(key.length).toBe(keyLength)
  })

  test('It should create a setup message', async () => {
    const PSI = await PSICardinality()
    const server = PSI.server!.createWithNewKey()
    const fpr = 0.001
    const numClientElements = 10
    const serverInputs = Array.from({ length: 100 }, (_, i) => `Element ${i}`)

    const setup = server.createSetupMessage(
      fpr,
      numClientElements,
      serverInputs
    )
    const parsed = JSON.parse(setup)

    expect(typeof parsed.num_hash_functions).toStrictEqual('number')
    expect(typeof parsed.bits).toStrictEqual('string')
  })

  test('It should throw if attempting to create a setup message after deletion', async () => {
    const PSI = await PSICardinality()
    const server = PSI.server!.createWithNewKey()
    const fpr = 0.001
    const numClientElements = 10
    const serverInputs = Array.from({ length: 100 }, (_, i) => `Element ${i}`)

    server.delete()

    expect(
      server.createSetupMessage.bind(
        server,
        fpr,
        numClientElements,
        serverInputs
      )
    ).toThrowError(ERROR_INSTANCE_DELETED)
  })

  test('It should fail to create a setup message', async () => {
    const PSI = await PSICardinality()
    const server = PSI.server!.createWithNewKey()
    const fpr = 0.001
    const numClientElements = -10
    const serverInputs = Array.from({ length: 100 }, (_, i) => `Element ${i}`)

    expect(
      server.createSetupMessage.bind(
        server,
        fpr,
        numClientElements,
        serverInputs
      )
    ).toThrow()
  })

  test('It should create a response to a client request', async () => {
    const PSI = await PSICardinality()
    const server = PSI.server!.createWithNewKey()
    const clientRequest = '["AiHdmxkmF/iOM0fFhny9917QYGcb9jq0GM9mP4L74ecM"]'

    const response = server.processRequest(clientRequest)

    expect(typeof response).toBe('string')
  })

  test('It should throw if attempting to create a response to a client request after deletion', async () => {
    const PSI = await PSICardinality()
    const server = PSI.server!.createWithNewKey()
    const clientRequest = '["AiHdmxkmF/iOM0fFhny9917QYGcb9jq0GM9mP4L74ecM"]'

    server.delete()

    expect(server.processRequest.bind(server, clientRequest)).toThrowError(
      ERROR_INSTANCE_DELETED
    )
  })

  test('It should fail to create a response from an invalid client request', async () => {
    const PSI = await PSICardinality()
    const server = PSI.server!.createWithNewKey()
    const clientRequest = 'invalid request'

    expect(server.processRequest.bind(server, clientRequest)).toThrow()
  })
})
