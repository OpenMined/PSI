const cpp_main = require('.');

(async () => {
    // Wait for the initializer to complete loading
    // Upon success, this will immediatly run our C++ main() function
    const { PSICardinality } = await cpp_main(`../../bazel-out/wasm-dbg/bin/psi_cardinality/javascript/psi_cardinality_server_${process.env.RUN_DEMO}.js`) // RUN_DEMO = js|wasm

    console.time('Creating server')
    const server = PSICardinality.PSICardinalityServer.CreateWithNewkey()
    console.timeEnd('Creating server')

    // TODO: Figure out why this causes a crash
    // console.time('Private key bytes')
    // // const bytes = server.GetPrivateKeyBytes()
    // console.timeEnd('Private key bytes')
    // console.log(`Key Bytes: '${bytes}'`)

    const numClientElements = 10
    const numServerElements = 100

    console.time('Server Inputs')
    const serverElements = Array.from({length: numServerElements}, (_, i) => `Element ${2 * i}`)
    const serverInputs = PSICardinality.vecFromJSArray(serverElements)
    console.timeEnd('Server Inputs')

    console.time('Server Setup Message')
    const setup = server.CreateSetupMessage(0.001, numClientElements, serverInputs)
    console.timeEnd('Server Setup Message')

    console.log(`setup: '${setup}'`)
    const clientRequest = '["AiHdmxkmF/iOM0fFhny9917QYGcb9jq0GM9mP4L74ecM","AiK1rXzYeZsUYwNR9lYXBBmYlcTFMqLCcHjcrsaGmjYv","ArWL4Au/vSMRwzMSoQp/wx79LMbE9QVXUuUs0fQC7+jV","As86QtTWj3zXwEaNUszxND+XCggown/JmLjOEAub6gNZ","AwQIw0Q2APjfh4pZuezodFarGUObZuRsnSHRt6UUyEIW","AxUyZOrfnfSsfoKn6WU+MogxV86O84wGfY4YeIDuODV4","A1pd7KrYwkpOQACwUW4dzIHqqJLGqy3KEkpRGZoyjOyN","A5WV5wuo7dtwJ5VCU1gZZ6UIVEZ4nPEGSSzML/F2jsaz","A8kWr10ssqPj5HuPaM0M+sT3yK2NcLivaeb6OL//0uYK","A9kFLnXzVbmd0eV37TtL1d5vNl5IkKdaM1KZuhfnmD8Z"]'

    const response = server.ProcessRequest(clientRequest)
    console.log('response', response)
})();