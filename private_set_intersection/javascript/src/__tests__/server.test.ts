import PSI from '../wasm_node'
import { ERROR_INSTANCE_DELETED } from '../implementation/constants'
import { PSILibrary } from '../implementation/psi'

let psi: PSILibrary
beforeAll(async () => {
  psi = await PSI()
})

describe('PSI Server', () => {
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
