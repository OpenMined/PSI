import PSICardinality from '../index_combined_wasm'
import { ERROR_INSTANCE_DELETED } from '../implementation/constants'
import { PSICardinalityLibrary } from 'src/implementation/psi_cardinality'

let PSI: PSICardinalityLibrary
beforeAll(async () => {
  PSI = await PSICardinality()
})

describe('PSI Client', () => {
  test('It should throw if deleted twice', async () => {
    const client = PSI.client!.create()

    client.delete()

    expect(client.delete).toThrowError(ERROR_INSTANCE_DELETED)
  })

  test('It should create a request', async () => {
    const client = PSI.client!.create()
    const numClientElements = 100
    const clientInputs = Array.from(
      { length: numClientElements },
      (_, i) => `Element ${i}`
    )

    const request = client.createRequest(clientInputs)
    const parsed = JSON.parse(request)

    expect(parsed.length).toStrictEqual(numClientElements)
  })

  test('It should throw if attempting to create a request after deletion', async () => {
    const client = PSI.client!.create()
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
    const client = PSI.client!.create()
    const serverSetup =
      '{"num_hash_functions":14,"bits":"1PnolAVduzqn+tXnoeyZ0ztdnwvAidt0U7gc2rWCtP5i6BDtSUydixXwGHP2mVAZ8PCNqGuTWUQO5VB0OEHTH8Cr1JhLxBDHRM4fAPEUvjjkBPNpsPRRtqzN2fW8y7rcANEwaRyInPsw1t4KYB0Q5vdy38VlPB1/Bz/adfGoGnXQ4fPNI/PN5s+zLcPev9odjAWyUQms8bvPHHsL2uyS7Uu5LRtOfdfYX5zWilxzNXECrabnnUnMqD5NH8c7oreTCWxwRQfYMxhf5K/EZTiTHphCi9O5Ey4ahwvNvykbvXdHQ7CDnLTKmomN6su5BrUt"}'
    const serverResponse =
      '["AiB2zX47wIFkO0VkHPiX4B2RPbzb2xre1/rcH3mARf6A","Aix7yJwCoTbF1RDFgLT7ntQ+DCdsxtQB5VKemNSbs95K","AlqATF16utJn0b2/mT4F/Tnxd7bQJUBc2em7H4OGkj7v","AnBfbcXx7YZ42/0zuIWyw0xECaf2bzlPWPe1TpD8LC1G","ArJt9KivSQERWNCAyUt1dAz4B2eud0qxnu206TJ0FxoT","AxfuH374m21PsyxFKQY9J2Seq/abvQv1cqDse8+f1DCv","AyHI21aQJyaa9HM3rPhhGt99iKHNg5wnwVeGwcmGggun","A0lFTmKUvyD7JzzR9J+syz+myC2metGmKb34FS4AkDQx","A2XvsIdnkPF6PJ6tPRI2LTdxguChs83Vd1VTM3DQXkdX","A606MaSef27r/KxUa1iRVWaX7lZqioxfz2iFpOR+6Yts"]'

    const intersection = client.processResponse(serverSetup, serverResponse)

    expect(intersection).toStrictEqual(0)
  })

  test('It should throw if attempting to process a response after deletion', async () => {
    const client = PSI.client!.create()
    const serverSetup =
      '{"num_hash_functions":14,"bits":"1PnolAVduzqn+tXnoeyZ0ztdnwvAidt0U7gc2rWCtP5i6BDtSUydixXwGHP2mVAZ8PCNqGuTWUQO5VB0OEHTH8Cr1JhLxBDHRM4fAPEUvjjkBPNpsPRRtqzN2fW8y7rcANEwaRyInPsw1t4KYB0Q5vdy38VlPB1/Bz/adfGoGnXQ4fPNI/PN5s+zLcPev9odjAWyUQms8bvPHHsL2uyS7Uu5LRtOfdfYX5zWilxzNXECrabnnUnMqD5NH8c7oreTCWxwRQfYMxhf5K/EZTiTHphCi9O5Ey4ahwvNvykbvXdHQ7CDnLTKmomN6su5BrUt"}'
    const serverResponse =
      '["AiB2zX47wIFkO0VkHPiX4B2RPbzb2xre1/rcH3mARf6A","Aix7yJwCoTbF1RDFgLT7ntQ+DCdsxtQB5VKemNSbs95K","AlqATF16utJn0b2/mT4F/Tnxd7bQJUBc2em7H4OGkj7v","AnBfbcXx7YZ42/0zuIWyw0xECaf2bzlPWPe1TpD8LC1G","ArJt9KivSQERWNCAyUt1dAz4B2eud0qxnu206TJ0FxoT","AxfuH374m21PsyxFKQY9J2Seq/abvQv1cqDse8+f1DCv","AyHI21aQJyaa9HM3rPhhGt99iKHNg5wnwVeGwcmGggun","A0lFTmKUvyD7JzzR9J+syz+myC2metGmKb34FS4AkDQx","A2XvsIdnkPF6PJ6tPRI2LTdxguChs83Vd1VTM3DQXkdX","A606MaSef27r/KxUa1iRVWaX7lZqioxfz2iFpOR+6Yts"]'

    client.delete()

    expect(
      client.processResponse.bind(client, serverSetup, serverResponse)
    ).toThrowError(ERROR_INSTANCE_DELETED)
  })

  test('It should fail to process a response', async () => {
    const client = PSI.client!.create()
    const serverSetup = 'invalid'
    const serverResponse = 'invalid'

    expect(
      client.processResponse.bind(client, serverSetup, serverResponse)
    ).toThrow()
  })
})
