import PSI from '../index_combined_wasm'
import { ERROR_INSTANCE_DELETED } from '../implementation/constants'
import { PSILibrary } from 'src/implementation/psi'

let psi: PSILibrary
beforeAll(async () => {
  psi = await PSI()
})

describe('PSI Server', () => {
  test('It should create from an existing key', async () => {
    const server1 = psi.server!.createWithNewKey()
    const key = server1.getPrivateKeyBytes()

    const server2 = psi.server!.createFromKey(key)

    expect(server2.getPrivateKeyBytes()).toEqual(key)
  })

  test('It should fail to create from an invalid key', async () => {
    const key = Uint8Array.from({ length: 32 })
    expect(() => psi.server!.createFromKey(key)).toThrow()
  })

  test('It should throw if deleted twice', async () => {
    const server = psi.server!.createWithNewKey()
    server.delete()
    expect(server.delete).toThrowError(ERROR_INSTANCE_DELETED)
  })

  test('It should return the private key as a binary array', async () => {
    const server = psi.server!.createWithNewKey()
    const keyLength = 32
    const key = server.getPrivateKeyBytes()
    expect(key.length).toBe(keyLength)
  })

  test('It should fail return the private key as a binary array if deleted', async () => {
    const server = psi.server!.createWithNewKey()
    server.delete()
    expect(() => server.getPrivateKeyBytes()).toThrowError(
      ERROR_INSTANCE_DELETED
    )
  })

  test('It should create a setup message', async () => {
    const server = psi.server!.createWithNewKey()
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
    const server = psi.server!.createWithNewKey()
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
    const server = psi.server!.createWithNewKey()
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
    const server = psi.server!.createWithNewKey()
    const clientRequest = JSON.stringify({
      // eslint-disable-next-line @typescript-eslint/camelcase
      encrypted_elements: [
        'A1mUKF2swn9cLscrtLPQarciqYD0le+9cP3bPOf006cz',
        'AwUjBPkHffyNsccAcSzBl5M1E5iORXKanYFa/4FDTNJx',
        'A6WVxMrgmvuNHwyO9G+fARpCtNUYDQozwsTmsDZ9zNd8',
        'A3g+/4ch3ZHPLBmjOR0/TiE+akLXb9+g1qNtxvH4bZ9p',
        'Aw3m8f2NLcoTN5xafJRZod2KtcnjqIBJl3BdJdJ7VVdf',
        'AzAloVV1YT5hQKYSdpOAFe181fI/dcemnNp27XXqjMYh',
        'A6W7mcZic2UHpv7C1J/CWa6p4SU0jpRPAX9w3Naqyok6',
        'Ag0dn7ieT96N70OWuKaM+4/6f+08oHMy0x/rxuCOgzpb',
        'At4Cce91xzLJ8ZCnYJjKiuE8sJ6KHDh6bod8A43Ns9a7',
        'ApbiHuf0mviZL6WhLwLK0czxk7qtfRgH9vks5eunzCMG'
      ],
      // eslint-disable-next-line @typescript-eslint/camelcase
      reveal_intersection: false
    })

    const response = server.processRequest(clientRequest)
    expect(typeof response).toBe('string')
  })

  test('It should throw if attempting to create a response to a client request after deletion', async () => {
    const server = psi.server!.createWithNewKey()
    const clientRequest = JSON.stringify({
      // eslint-disable-next-line @typescript-eslint/camelcase
      encrypted_elements: [
        'A1mUKF2swn9cLscrtLPQarciqYD0le+9cP3bPOf006cz',
        'AwUjBPkHffyNsccAcSzBl5M1E5iORXKanYFa/4FDTNJx',
        'A6WVxMrgmvuNHwyO9G+fARpCtNUYDQozwsTmsDZ9zNd8',
        'A3g+/4ch3ZHPLBmjOR0/TiE+akLXb9+g1qNtxvH4bZ9p',
        'Aw3m8f2NLcoTN5xafJRZod2KtcnjqIBJl3BdJdJ7VVdf',
        'AzAloVV1YT5hQKYSdpOAFe181fI/dcemnNp27XXqjMYh',
        'A6W7mcZic2UHpv7C1J/CWa6p4SU0jpRPAX9w3Naqyok6',
        'Ag0dn7ieT96N70OWuKaM+4/6f+08oHMy0x/rxuCOgzpb',
        'At4Cce91xzLJ8ZCnYJjKiuE8sJ6KHDh6bod8A43Ns9a7',
        'ApbiHuf0mviZL6WhLwLK0czxk7qtfRgH9vks5eunzCMG'
      ],
      // eslint-disable-next-line @typescript-eslint/camelcase
      reveal_intersection: false
    })

    server.delete()

    expect(server.processRequest.bind(server, clientRequest)).toThrowError(
      ERROR_INSTANCE_DELETED
    )
  })

  test('It should fail to create a response from an invalid client request', async () => {
    const server = psi.server!.createWithNewKey()
    const clientRequest = 'invalid request'

    expect(server.processRequest.bind(server, clientRequest)).toThrow()
  })
})
