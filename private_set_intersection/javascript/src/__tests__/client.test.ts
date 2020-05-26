import PSI from '../index_combined_wasm'
import { ERROR_INSTANCE_DELETED } from '../implementation/constants'
import { PSILibrary } from 'src/implementation/psi'

let psi: PSILibrary
beforeAll(async () => {
  psi = await PSI()
})

describe('PSI Client', () => {
  test('It should throw if deleted twice', async () => {
    const client = psi.client!.create()

    client.delete()

    expect(client.delete).toThrowError(ERROR_INSTANCE_DELETED)
  })

  test('It should create a request', async () => {
    const client = psi.client!.create()
    const numClientElements = 10
    const clientInputs = Array.from(
      { length: numClientElements },
      (_, i) => `Element ${i}`
    )

    const request = client.createRequest(clientInputs)
    const parsed = JSON.parse(request)
    expect(parsed.encrypted_elements.length).toStrictEqual(numClientElements)
  })

  test('It should throw if attempting to create a request after deletion', async () => {
    const client = psi.client!.create()
    const numClientElements = 100
    const clientInputs = Array.from(
      { length: numClientElements },
      (_, i) => `Element ${i}`
    )

    client.delete()

    expect(client.createRequest.bind(client, clientInputs)).toThrowError(
      ERROR_INSTANCE_DELETED
    )
  })

  test('It should process a response', async () => {
    const client = psi.client!.create()
    const serverSetup = JSON.stringify({
      // eslint-disable-next-line @typescript-eslint/camelcase
      num_hash_functions: 14,
      bits:
        '1PnolAVduzqn+tXnoeyZ0ztdnwvAidt0U7gc2rWCtP5i6BDtSUydixXwGHP2mVAZ8PCNqGuTWUQO5VB0OEHTH8Cr1JhLxBDHRM4fAPEUvjjkBPNpsPRRtqzN2fW8y7rcANEwaRyInPsw1t4KYB0Q5vdy38VlPB1/Bz/adfGoGnXQ4fPNI/PN5s+zLcPev9odjAWyUQms8bvPHHsL2uyS7Uu5LRtOfdfYX5zWilxzNXECrabnnUnMqD5NH8c7oreTCWxwRQfYMxhf5K/EZTiTHphCi9O5Ey4ahwvNvykbvXdHQ7CDnLTKmomN6su5BrUt'
    })
    const serverResponse = JSON.stringify({
      // eslint-disable-next-line @typescript-eslint/camelcase
      encrypted_elements: [
        'Aw8M6ufg9yUnMdHXQKqZ+2g4Z2uhLzn2J/7dKnp/uDEL',
        'Av/dJyi/E7gunWLvXX2A1AcnU6dEmB40igRCrqskGTgB',
        'Av8i8ltywvz4VRFTFju//VoavSL5Fmt95wWd8ChPCrCS',
        'A1kEdR6hNbGz1sQfHSInQrbZfpDTu6OPiQbnEisAA1wl',
        'Ap/z+I2cPqyUxM0YYlLOpKtRJYaSMfgNcyRY1Jynun0r',
        'A6CpgzV4+F6Jjr+yOYbLdpkphMVTjoRuiZOP8uI+7he3',
        'AnQOkG1+kkYuMvtOcQPqV5YKTYoVe/P+/cUHrfaWDJaD',
        'An8POEQwL/cEHmQrwAdoBSPYh6a6jPOthH3vBx+2owOF',
        'A23B3m3hJtAi607Z8FKF/P4fXCTdMCPzOjxgCFlna9K6',
        'AqV+jTb23uTHPDQmPZsEuxDm5kKYIuI9pkEjvM8fMTG9'
      ]
    })
    const intersection = client.getIntersectionSize(serverSetup, serverResponse)

    expect(intersection).toStrictEqual(0)
  })

  test('It should throw if attempting to process a response after deletion', async () => {
    const client = psi.client!.create()
    const serverSetup = JSON.stringify({
      // eslint-disable-next-line @typescript-eslint/camelcase
      num_hash_functions: 14,
      bits:
        '1PnolAVduzqn+tXnoeyZ0ztdnwvAidt0U7gc2rWCtP5i6BDtSUydixXwGHP2mVAZ8PCNqGuTWUQO5VB0OEHTH8Cr1JhLxBDHRM4fAPEUvjjkBPNpsPRRtqzN2fW8y7rcANEwaRyInPsw1t4KYB0Q5vdy38VlPB1/Bz/adfGoGnXQ4fPNI/PN5s+zLcPev9odjAWyUQms8bvPHHsL2uyS7Uu5LRtOfdfYX5zWilxzNXECrabnnUnMqD5NH8c7oreTCWxwRQfYMxhf5K/EZTiTHphCi9O5Ey4ahwvNvykbvXdHQ7CDnLTKmomN6su5BrUt'
    })
    const serverResponse = JSON.stringify({
      // eslint-disable-next-line @typescript-eslint/camelcase
      encrypted_elements: [
        'Aw8M6ufg9yUnMdHXQKqZ+2g4Z2uhLzn2J/7dKnp/uDEL',
        'Av/dJyi/E7gunWLvXX2A1AcnU6dEmB40igRCrqskGTgB',
        'Av8i8ltywvz4VRFTFju//VoavSL5Fmt95wWd8ChPCrCS',
        'A1kEdR6hNbGz1sQfHSInQrbZfpDTu6OPiQbnEisAA1wl',
        'Ap/z+I2cPqyUxM0YYlLOpKtRJYaSMfgNcyRY1Jynun0r',
        'A6CpgzV4+F6Jjr+yOYbLdpkphMVTjoRuiZOP8uI+7he3',
        'AnQOkG1+kkYuMvtOcQPqV5YKTYoVe/P+/cUHrfaWDJaD',
        'An8POEQwL/cEHmQrwAdoBSPYh6a6jPOthH3vBx+2owOF',
        'A23B3m3hJtAi607Z8FKF/P4fXCTdMCPzOjxgCFlna9K6',
        'AqV+jTb23uTHPDQmPZsEuxDm5kKYIuI9pkEjvM8fMTG9'
      ]
    })
    client.delete()

    expect(
      client.getIntersectionSize.bind(client, serverSetup, serverResponse)
    ).toThrowError(ERROR_INSTANCE_DELETED)
  })

  test('It should fail to process a response', async () => {
    const client = psi.client!.create()
    const serverSetup = 'invalid'
    const serverResponse = 'invalid'

    expect(
      client.getIntersectionSize.bind(client, serverSetup, serverResponse)
    ).toThrow()
  })
})
