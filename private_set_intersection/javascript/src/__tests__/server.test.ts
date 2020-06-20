import PSI from '../index_combined_wasm'
import { ERROR_INSTANCE_DELETED } from '../implementation/constants'
import { PSILibrary } from 'src/implementation/psi'
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

    const numHashFunctions = setup.getNumHashFunctions()
    expect(numHashFunctions).toStrictEqual(14)
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

  test.skip('It should fail to create a response from an invalid client request', async () => {
    const server = psi.server!.createWithNewKey()

    // prettier-ignore
    const request = Request.deserializeBinary(Uint8Array.from([
      0,  0,   0,   0, 0,  0,  0, 0, 0, 0,  0,  0,
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

    expect(server.processRequest.bind(server, request)).toThrow()
  })
})
