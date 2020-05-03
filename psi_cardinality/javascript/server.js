const cpp_main = require('.');

(async () => {
    // Wait for the initializer to complete loading
    // Upon success, this will immediatly run our C++ main() function
    const { PSICardinality } = await cpp_main(`../../bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_server_${process.env.RUN_DEMO}.js`) // RUN_DEMO = js|wasm

    
    const server = PSICardinality.PSICardinalityServer.CreateWithNewkey()

    const inputs = PSICardinality.vecFromJSArray(['hello', 'world'])

    // TODO: Finish server binding tests
    // const setup = server.CreateSetupMessage(0.001, inputs.size(), inputs)
    // console.log('setup', setup)

})();