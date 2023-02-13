import PSI from '../wasm_node'
import { ERROR_INSTANCE_DELETED } from '../implementation/constants'
import { PSILibrary } from '../implementation/psi'

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
    expect(server.delete).toThrow(ERROR_INSTANCE_DELETED)
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
    expect(() => server.getPrivateKeyBytes()).toThrow(ERROR_INSTANCE_DELETED)
  })

  test('It should create a setup message (BloomFilter)', async () => {
    const server = psi.server!.createWithNewKey()
    const fpr = 0.01
    const numClientElements = 10
    const serverInputs = Array.from({ length: 100 }, (_, i) => `Element ${i}`)

    const setup = server.createSetupMessage(
      fpr,
      numClientElements,
      serverInputs,
      psi.dataStructure.BloomFilter
    )

    const numHashFunctions = setup.getBloomFilter()!.getNumHashFunctions()
    expect(numHashFunctions).toStrictEqual(10)
    const bits = setup.getBloomFilter()!.getBits()
    expect(bits.constructor).toStrictEqual(Uint8Array)
  })

  test('It should create a setup message (GCS)', async () => {
    const server = psi.server!.createWithNewKey()
    const fpr = 0.01
    const numClientElements = 10
    const serverInputs = Array.from({ length: 100 }, (_, i) => `Element ${i}`)

    const setup = server.createSetupMessage(
      fpr,
      numClientElements,
      serverInputs,
      psi.dataStructure.GCS
    )

    const hashRange = setup.getGcs()!.getHashRange()
    expect(hashRange).toStrictEqual(100000)
    const bits = setup.getGcs()!.getBits()
    expect(bits.constructor).toStrictEqual(Uint8Array)
  })

  test('It should create a setup message (Raw)', async () => {
    const server = psi.server!.createWithNewKey()
    const fpr = 0.01
    const numClientElements = 10
    const serverInputs = Array.from({ length: 100 }, (_, i) => `Element ${i}`)

    const setup = server.createSetupMessage(
      fpr,
      numClientElements,
      serverInputs,
      psi.dataStructure.Raw
    )

    const bits = setup.getRaw()!.getEncryptedElementsList()
    expect(bits.constructor).toStrictEqual(Array)
    expect(bits.length).toStrictEqual(100)
  })

  test('It should throw if attempting to create a setup message after deletion', async () => {
    const server = psi.server!.createWithNewKey()
    const fpr = 0.01
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
    ).toThrow(ERROR_INSTANCE_DELETED)
  })

  test('It should fail to create a setup message', async () => {
    const server = psi.server!.createWithNewKey()
    const fpr = 0.01
    const numClientElements = Infinity
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
})
