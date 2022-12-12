import PSI from '../combined_wasm_node'
import { ERROR_INSTANCE_DELETED } from '../implementation/constants'
import { PSILibrary } from '../implementation/psi'
import { Request, Response } from '../implementation/proto/psi_pb'

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

  test('It should create a setup message (BloomFilter)', async () => {
    const server = psi.server!.createWithNewKey()
    const fpr = 0.001
    const numClientElements = 10
    const serverInputs = Array.from({ length: 100 }, (_, i) => `Element ${i}`)

    const setup = server.createSetupMessage(
      fpr,
      numClientElements,
      serverInputs,
      psi.dataStructure.BloomFilter
    )

    const numHashFunctions = setup.getBloomFilter()!.getNumHashFunctions()
    expect(numHashFunctions).toStrictEqual(14)
    const bits = setup.getBits()
    expect(bits.constructor).toStrictEqual(Uint8Array)
  })

  test('It should create a setup message (GCS)', async () => {
    const server = psi.server!.createWithNewKey()
    const fpr = 0.001
    const numClientElements = 10
    const serverInputs = Array.from({ length: 100 }, (_, i) => `Element ${i}`)

    const setup = server.createSetupMessage(
      fpr,
      numClientElements,
      serverInputs,
      psi.dataStructure.GCS
    )

    const hashRange = setup.getGcs()!.getHashRange()
    expect(hashRange).toStrictEqual(1000000)
    const bits = setup.getBits()
    expect(bits.constructor).toStrictEqual(Uint8Array)
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

  test('It should create a response to a client request', async () => {
    const server = psi.server!.createWithNewKey()

    // prettier-ignore
    const request = Request.deserializeBinary(Uint8Array.from([
      18,  33,   2,   3, 219,  96,  37, 182, 133, 228,  25,  79,
     234,  44, 142,  91,  90,  22, 254,  26,  85, 201,   1, 156,
      53,  47, 231, 118,  42, 240, 188,  89, 160, 180, 240,  18,
      33,   3,  38, 205,  60, 197,  53, 232,  56, 179, 140, 157,
     110, 175, 219,  95, 190, 185, 219,  33,  40, 221, 106,  42,
     159,  57,  21, 166, 225, 111, 183, 142,  25, 217,  18,  33,
       2, 137,  13, 129, 117,  62,  62, 141,  34,   1, 108, 181,
     149, 219,  90,  51,  13,  24,   5, 198,  81,  75, 114,  57,
      50,  14,  83, 176, 247, 251, 230, 174, 151,  18,  33,   3,
     174, 135,   0, 184, 126, 207, 121,  90, 221, 248, 120, 116,
     212, 125,  74, 199, 188, 206,  64, 171,  44, 140,  29,  33,
     204,  37,  51,  60, 187, 171, 190, 140,  18,  33,   3, 107,
     179, 129, 236, 175, 208, 179,  90, 102,  86, 209, 237,  32,
     176, 187,  99, 156, 237,  50, 193, 107, 226,  98, 175,  90,
       1, 179,  52, 192, 248,  92, 244,  18,  33,   3,  96,  57,
     220, 207, 167, 203,  19,  11, 120, 189,  33,  69, 249,  66,
     156, 188, 251,   5, 133, 148,  32, 230,  76,  66, 199, 151,
     178, 152,  50, 176, 121, 253,  18,  33,   2, 108,  40,  24,
     235,  61, 178,  53,  93,  59, 221, 145,  50, 205, 218, 118,
     244, 167, 118, 204, 238,  51,  93,  46, 189, 168,  41,  50,
     144, 106, 226, 185,  77,  18,  33,   2, 137, 185, 176, 205,
      77, 212, 199,  54, 193,  72, 141,  23, 252, 194, 187,  27,
     150, 197, 124, 201, 183,   7, 100,  43, 197, 142,   6, 138,
      96,  73,  11,  38,  18,  33,   2, 101, 115, 165, 127, 201,
      74, 239,  33, 103,  94,  18,  43,  62,  50,  17, 223,  83,
     177,  25,  23, 196, 200, 192, 198, 115,  40,  84,  90, 137,
     228, 118,  16,  18,  33,   2,  82,  37, 166, 155,   4, 244,
     228, 122, 150, 169, 225,  71, 110, 213,  42,  18, 105, 146,
      58, 180, 149, 197,  47, 214,  81, 126, 113, 248,   5,  16,
      19, 194
    ]))

    const response = server.processRequest(request)
    expect(response.constructor).toBe(Response)
    const numEncryptedElements = response.getEncryptedElementsList()
    expect(numEncryptedElements.length).toBe(10)
  })

  test('It should throw if attempting to create a response to a client request after deletion', async () => {
    const server = psi.server!.createWithNewKey()

    // prettier-ignore
    const request = Request.deserializeBinary(Uint8Array.from([
      18,  33,   2,   3, 219,  96,  37, 182, 133, 228,  25,  79,
     234,  44, 142,  91,  90,  22, 254,  26,  85, 201,   1, 156,
      53,  47, 231, 118,  42, 240, 188,  89, 160, 180, 240,  18,
      33,   3,  38, 205,  60, 197,  53, 232,  56, 179, 140, 157,
     110, 175, 219,  95, 190, 185, 219,  33,  40, 221, 106,  42,
     159,  57,  21, 166, 225, 111, 183, 142,  25, 217,  18,  33,
       2, 137,  13, 129, 117,  62,  62, 141,  34,   1, 108, 181,
     149, 219,  90,  51,  13,  24,   5, 198,  81,  75, 114,  57,
      50,  14,  83, 176, 247, 251, 230, 174, 151,  18,  33,   3,
     174, 135,   0, 184, 126, 207, 121,  90, 221, 248, 120, 116,
     212, 125,  74, 199, 188, 206,  64, 171,  44, 140,  29,  33,
     204,  37,  51,  60, 187, 171, 190, 140,  18,  33,   3, 107,
     179, 129, 236, 175, 208, 179,  90, 102,  86, 209, 237,  32,
     176, 187,  99, 156, 237,  50, 193, 107, 226,  98, 175,  90,
       1, 179,  52, 192, 248,  92, 244,  18,  33,   3,  96,  57,
     220, 207, 167, 203,  19,  11, 120, 189,  33,  69, 249,  66,
     156, 188, 251,   5, 133, 148,  32, 230,  76,  66, 199, 151,
     178, 152,  50, 176, 121, 253,  18,  33,   2, 108,  40,  24,
     235,  61, 178,  53,  93,  59, 221, 145,  50, 205, 218, 118,
     244, 167, 118, 204, 238,  51,  93,  46, 189, 168,  41,  50,
     144, 106, 226, 185,  77,  18,  33,   2, 137, 185, 176, 205,
      77, 212, 199,  54, 193,  72, 141,  23, 252, 194, 187,  27,
     150, 197, 124, 201, 183,   7, 100,  43, 197, 142,   6, 138,
      96,  73,  11,  38,  18,  33,   2, 101, 115, 165, 127, 201,
      74, 239,  33, 103,  94,  18,  43,  62,  50,  17, 223,  83,
     177,  25,  23, 196, 200, 192, 198, 115,  40,  84,  90, 137,
     228, 118,  16,  18,  33,   2,  82,  37, 166, 155,   4, 244,
     228, 122, 150, 169, 225,  71, 110, 213,  42,  18, 105, 146,
      58, 180, 149, 197,  47, 214,  81, 126, 113, 248,   5,  16,
      19, 194
    ]))

    server.delete()

    expect(server.processRequest.bind(server, request)).toThrowError(
      ERROR_INSTANCE_DELETED
    )
  })

  test('It should fail to create a response from an invalid client request', async () => {
    const server = psi.server!.createWithNewKey()

    // prettier-ignore
    const request = Request.deserializeBinary(Uint8Array.from([
        8,   1,  18,  33,   3,  64,  96, 212, 146, 147, 255, 186,
      229, 165,   6,  66, 251, 234, 144, 164, 222, 156,  79, 150,
      116,  19, 191,  20,  22, 123, 236, 249, 152, 129,  29,  48,
      39,  18,  33,   2,  85, 204, 236, 125, 122, 245, 142,  39,
      102, 112,  47, 176,  44, 106, 183,  57, 183, 166, 174, 193,
        8, 246,  39,  87, 187, 244, 128, 253,  81, 114,  74, 131,
      18,  33,   3, 185,  14,   6,  28,  45, 130, 114, 213, 147,
      164, 227,  41, 250, 236,  65, 228, 228, 141,  26, 187, 176,
      103,  53, 245,  48, 250, 130, 184, 200, 181, 123, 181,  18,
      33,   3,  54, 190,  87, 246, 119,  32,  55, 243,  46,  48,
      84, 222, 164, 210,  17, 225, 185, 138, 154,  41, 224, 123,
      169, 152,  48, 237,  95, 134, 105, 172,  59, 111,  18,  33,
        2, 104,  36, 143, 103,  32, 239, 174,  80, 151,  97, 167,
      46, 156, 224, 196, 189, 119,  48,  44,  36,  69,  56,  26,
      232,  84,  71, 234, 237,  41, 184, 243,  85,  18,  33,   3,
      148, 214, 221, 236, 174,  68, 225,  28, 105,  32, 230, 224,
      111,  35,  25, 201, 189, 119,  26, 105,  31, 196, 140, 139,
      129, 193,  75, 117, 148, 148, 120, 100,  18,  33,   3, 173,
      254, 199,   3, 219, 173, 178, 187, 198,  22,  44,  30,  93,
      43, 160, 189, 134, 151, 197,  62, 165,   0, 197,  95,   5,
      48, 121, 151,  12, 106, 190, 240,  18,  33,   3,  86, 233,
      172, 104,  61, 190, 213, 154, 143,  19, 197,  68, 240, 236,
      77, 227, 217, 164,  86, 109, 233,  78,   0,  57, 119, 109,
      37, 183, 214,   4, 140,  12,  18,  33,   2,   6,  90,  46,
      46, 239, 113, 156, 252, 161, 131,  65, 193, 164, 167,  24,
      68,  42, 244,  53,  64, 181, 255, 165, 195, 110, 195, 115,
      77, 101, 118, 141,  22,  18,  33,   3,  25, 105, 238,  92,
      196, 202, 105, 252,   5, 193, 111,  98, 184, 166,  16,  12,
      129,  97, 240,  52,  11, 151,  40, 168, 218, 226,  13, 188,
      231,  92,  27, 247
    ]))
    expect(server.processRequest.bind(server, request)).toThrow()
  })
})
