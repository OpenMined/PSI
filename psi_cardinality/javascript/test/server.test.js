import PSICardinality from '../src/index_combined_wasm'

let Server = null
beforeAll(async () => {
  Server = await PSICardinality.Server.CreateWithNewKey()
})

describe('PSI Server', () => {
  test('It should create from an existing key', async () => {
    const server2 = await PSICardinality.Server.CreateWithNewKey()
    const key = server2.GetPrivateKeyBytes()
    const spyOn = jest.spyOn(PSICardinality.Server, 'CreateFromKey')
    const server = await PSICardinality.Server.CreateFromKey(key)
    expect(spyOn).toHaveBeenCalledWith(key)
    expect(server).toBeInstanceOf(Object)
    expect(server.constructor).toBe(Object)
    expect(server.constructor.name).toBe('Object')
    expect(server.GetPrivateKeyBytes()).toEqual(key)
  })
  test('It should fail to create from an invalid key', async () => {
    const key = Uint8Array.from({ length: 32 })
    const spyOn = jest.spyOn(PSICardinality.Server, 'CreateFromKey')
    await expect(PSICardinality.Server.CreateFromKey(key)).rejects.toThrow()
    expect(spyOn).toHaveBeenCalledWith(key)
  })
  test("It should delete it's instance", async () => {
    const server = await PSICardinality.Server.CreateWithNewKey()
    const spyOn = jest.spyOn(server, 'delete')
    server.delete()
    expect(spyOn).toHaveBeenCalled()
  })
  test('It should return the private key as a binary array', () => {
    const spyOn = jest.spyOn(Server, 'GetPrivateKeyBytes')
    const key = Server.GetPrivateKeyBytes()
    expect(spyOn).toHaveBeenCalled()
    expect(key.constructor).toBe(Uint8Array)
  })
  test('It should create a setup message', () => {
    const fpr = 0.001
    const numClientElements = 10
    const serverInputs = Array.from({ length: 100 }, (_, i) => `Element ${i}`)

    const spyOn = jest.spyOn(Server, 'CreateSetupMessage')
    const setup = Server.CreateSetupMessage(
      fpr,
      numClientElements,
      serverInputs
    )
    expect(spyOn).toHaveBeenCalledWith(fpr, numClientElements, serverInputs)
    expect(typeof setup).toBe('string')
  })
  test('It should fail to create a setup message', () => {
    const fpr = 0.001
    const numClientElements = -10
    const serverInputs = Array.from({ length: 100 }, (_, i) => `Element ${i}`)

    const spyOn = jest.spyOn(Server, 'CreateSetupMessage')
    expect(() =>
      Server.CreateSetupMessage(fpr, numClientElements, serverInputs)
    ).toThrow()
    expect(spyOn).toHaveBeenCalledWith(fpr, numClientElements, serverInputs)
  })
  test('It should create a response to a client request', () => {
    const clientRequest = '["AiHdmxkmF/iOM0fFhny9917QYGcb9jq0GM9mP4L74ecM"]'
    const spyOn = jest.spyOn(Server, 'ProcessRequest')
    const response = Server.ProcessRequest(clientRequest)
    expect(spyOn).toHaveBeenCalledWith(clientRequest)
    expect(typeof response).toBe('string')
  })
  test('It should fail to create a response from an invalid client request', () => {
    const clientRequest = 'invalid request'
    const spyOn = jest.spyOn(Server, 'ProcessRequest')
    expect(() => Server.ProcessRequest(clientRequest)).toThrow()
    expect(spyOn).toHaveBeenCalledWith(clientRequest)
  })
})
