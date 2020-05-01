const cpp_main = require('./');

(async () => {
    // Wait for the initializer to complete loading
    // Upon success, this will immediatly run our C++ main() function
    await cpp_main(process.env.RUN_DEMO) // RUN_DEMO = js|wasm
})();