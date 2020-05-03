const cpp_main = require('.');

(async () => {
    // Wait for the initializer to complete loading
    // Upon success, this will immediatly run our C++ main() function
    const { PSICardinality } = await cpp_main(`../../bazel-out/wasm-opt/bin/src/javascript/psi_cardinality_client_${process.env.RUN_DEMO}.js`) // RUN_DEMO = js|wasm

    const vector = PSICardinality.vecFromJSArray(['hello', 'world'])
    
    const client = PSICardinality.PSICardinalityClient.Create()

    const request = client.CreateRequest(vector)
    console.log('request', request)

})();