import cpp_main from '.'
;(async () => {
  // Wait for the initializer to complete loading
  // Upon success, this will immediatly run our C++ main() function
  await cpp_main(
    `../../bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_benchmark_${process.env.RUN_DEMO}.js`
  ) // RUN_DEMO = js|wasm
})()
