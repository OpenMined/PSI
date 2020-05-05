import PSICardinality from '../src/index_combined_wasm'
import { Client } from '../src/implementation/client'

let ClientInstance: Client

beforeAll(async () => {
  ClientInstance = await PSICardinality.Client.Create()
})

describe('PSI Client', () => {
  test("It should delete it's instance", async () => {
    const client = await PSICardinality.Client.Create()
    const spyOn = jest.spyOn(client, 'delete')
    client.delete()
    expect(spyOn).toHaveBeenCalled()
  })
  test('It should create a request', () => {
    const numClientElements = 100
    const clientInputs = Array.from(
      { length: numClientElements },
      (_, i) => `Element ${i}`
    )
    const request = ClientInstance.CreateRequest(clientInputs)
    expect(typeof request).toBe('string')
  })
  // test('It should fail to create a request', () => {
  //   const numClientElements = 10
  //   const clientInputs = Array.from({ length: numClientElements }, (_, i) => i)
  //   const spyOn = jest.spyOn(ClientInstance, 'CreateRequest')
  //   expect(() => ClientInstance.CreateRequest(clientInputs)).toThrow()
  //   expect(spyOn).toHaveBeenCalled()
  // })
  test('It should process a response', () => {
    const serverSetup =
      '{"num_hash_functions":14,"bits":"1PnolAVduzqn+tXnoeyZ0ztdnwvAidt0U7gc2rWCtP5i6BDtSUydixXwGHP2mVAZ8PCNqGuTWUQO5VB0OEHTH8Cr1JhLxBDHRM4fAPEUvjjkBPNpsPRRtqzN2fW8y7rcANEwaRyInPsw1t4KYB0Q5vdy38VlPB1/Bz/adfGoGnXQ4fPNI/PN5s+zLcPev9odjAWyUQms8bvPHHsL2uyS7Uu5LRtOfdfYX5zWilxzNXECrabnnUnMqD5NH8c7oreTCWxwRQfYMxhf5K/EZTiTHphCi9O5Ey4ahwvNvykbvXdHQ7CDnLTKmomN6su5BrUt"}'
    const serverResponse =
      '["AiB2zX47wIFkO0VkHPiX4B2RPbzb2xre1/rcH3mARf6A","Aix7yJwCoTbF1RDFgLT7ntQ+DCdsxtQB5VKemNSbs95K","AlqATF16utJn0b2/mT4F/Tnxd7bQJUBc2em7H4OGkj7v","AnBfbcXx7YZ42/0zuIWyw0xECaf2bzlPWPe1TpD8LC1G","ArJt9KivSQERWNCAyUt1dAz4B2eud0qxnu206TJ0FxoT","AxfuH374m21PsyxFKQY9J2Seq/abvQv1cqDse8+f1DCv","AyHI21aQJyaa9HM3rPhhGt99iKHNg5wnwVeGwcmGggun","A0lFTmKUvyD7JzzR9J+syz+myC2metGmKb34FS4AkDQx","A2XvsIdnkPF6PJ6tPRI2LTdxguChs83Vd1VTM3DQXkdX","A606MaSef27r/KxUa1iRVWaX7lZqioxfz2iFpOR+6Yts"]'

    const spyOn = jest.spyOn(ClientInstance, 'ProcessResponse')
    const intersection = ClientInstance.ProcessResponse(
      serverSetup,
      serverResponse
    )
    expect(spyOn).toHaveBeenCalled()
    expect(typeof intersection).toBe('number')
  })
  test('It should fail to process a response', () => {
    const serverSetup = 'invalid'
    const serverResponse = 'invalid'
    const spyOn = jest.spyOn(ClientInstance, 'ProcessResponse')
    expect(() =>
      ClientInstance.ProcessResponse(serverSetup, serverResponse)
    ).toThrow()
    expect(spyOn).toHaveBeenCalled()
  })
})
