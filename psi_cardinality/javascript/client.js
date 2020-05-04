const cpp_main = require('.');

(async () => {
    // Wait for the initializer to complete loading
    // Upon success, this will immediatly run our C++ main() function
    const { PSICardinality } = await cpp_main(`../../bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_client_${process.env.RUN_DEMO}.js`) // RUN_DEMO = js|wasm

    const numClientElements = 10
    const clientElements = Array.from({length: numClientElements}, (_, i) => `Element ${i}`)
    const clientInputs = PSICardinality.VecFromJSArray(clientElements)

    const client = PSICardinality.PSICardinalityClient.Create()

    console.time('Client Create Request')
    const request = client.CreateRequest(clientInputs)
    console.timeEnd('Client Create Request')
    console.log(`Request: '${request}'`)

    const serverSetup = '{"num_hash_functions":14,"bits":"1PnolAVduzqn+tXnoeyZ0ztdnwvAidt0U7gc2rWCtP5i6BDtSUydixXwGHP2mVAZ8PCNqGuTWUQO5VB0OEHTH8Cr1JhLxBDHRM4fAPEUvjjkBPNpsPRRtqzN2fW8y7rcANEwaRyInPsw1t4KYB0Q5vdy38VlPB1/Bz/adfGoGnXQ4fPNI/PN5s+zLcPev9odjAWyUQms8bvPHHsL2uyS7Uu5LRtOfdfYX5zWilxzNXECrabnnUnMqD5NH8c7oreTCWxwRQfYMxhf5K/EZTiTHphCi9O5Ey4ahwvNvykbvXdHQ7CDnLTKmomN6su5BrUt"}'
    const serverResponse = '["AiB2zX47wIFkO0VkHPiX4B2RPbzb2xre1/rcH3mARf6A","Aix7yJwCoTbF1RDFgLT7ntQ+DCdsxtQB5VKemNSbs95K","AlqATF16utJn0b2/mT4F/Tnxd7bQJUBc2em7H4OGkj7v","AnBfbcXx7YZ42/0zuIWyw0xECaf2bzlPWPe1TpD8LC1G","ArJt9KivSQERWNCAyUt1dAz4B2eud0qxnu206TJ0FxoT","AxfuH374m21PsyxFKQY9J2Seq/abvQv1cqDse8+f1DCv","AyHI21aQJyaa9HM3rPhhGt99iKHNg5wnwVeGwcmGggun","A0lFTmKUvyD7JzzR9J+syz+myC2metGmKb34FS4AkDQx","A2XvsIdnkPF6PJ6tPRI2LTdxguChs83Vd1VTM3DQXkdX","A606MaSef27r/KxUa1iRVWaX7lZqioxfz2iFpOR+6Yts"]'
    const intersection = client.ProcessResponse(serverSetup, serverResponse)
    console.log(`intersection: ${intersection}`)
})();