const cpp_main = require('.');

(async () => {
    // Wait for the initializer to complete loading
    // Upon success, this will immediatly run our C++ main() function
    const { PSICardinality } = await cpp_main(`../../bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_client_${process.env.RUN_DEMO}.js`) // RUN_DEMO = js|wasm

    const numClientElements = 10
    const clientElements = Array.from({length: numClientElements}, (_, i) => `Element ${i}`)
    const clientInputs = PSICardinality.vecFromJSArray(clientElements)

    const client = PSICardinality.PSICardinalityClient.Create()

    console.time('Client Create Request')
    const request = client.CreateRequest(clientInputs)
    console.timeEnd('Client Create Request')
    console.log(`Request: '${request}'`)
})();