load("@emsdk//emscripten_toolchain:wasm_rules.bzl", "wasm_cc_binary")

package(default_visibility = ["//visibility:public"])

DEFAULT_EMSCRIPTEN_LINKOPTS = [
    "--bind",  # Compiles the source code using the Embind bindings to connect C/C++ and JavaScript
    "--closure 1",  # Run the closure compiler
    "-s MALLOC=emmalloc",  # Switch to using the much smaller implementation
    "-s ALLOW_MEMORY_GROWTH=1",  # Allow both asm.js and wasm to increase memory dynamically
    "-s USE_PTHREADS=0",  # Ensure we disable pthreads
    "-s ASSERTIONS=0",  # Turn off runtime assertions
    "-s USE_ES6_IMPORT_META=0",  # Disable loading from import meta since we use rollup
    "-s SINGLE_FILE=1",  # Pack all webassembly into base64
    "-s WASM_BIGINT=0",  # Disable WebAssembly integration with JavaScript BigInt as it isn't used
    "-s DISABLE_EXCEPTION_CATCHING=1",  # Disable all exception catching
    "-s NODEJS_CATCH_EXIT=0",  # We don't have a 'main' so disable exit() catching
    "-s WASM=1",  # Specify wasm ouptut
    "-s ERROR_ON_UNDEFINED_SYMBOLS=0",  # Ignore POSIX popen, __Unwind_GetIP
    "-s MODULARIZE=1",  # Allows us to manually invoke the initialization of wasm
]

NODE_LINKOPTS = [
    "-s EXPORT_ES6=0",
    "-s ENVIRONMENT=node",
]
WEB_LINKOPTS = [
    "-s EXPORT_ES6=1",  # Export as es6 module, used for rollup
    "-s ENVIRONMENT=web,webview",
]
WORKER_LINKOPTS = [
    "-s EXPORT_ES6=1",  # Export as es6 module, used for rollup
    "-s ENVIRONMENT=worker",
]


# Combined server + client build
cc_binary(
    name = "combined_wasm_node.js",
    srcs = [
        "utils.h",
        "client.cpp",
        "data_structure.cpp",
        "package.cpp",
        "server.cpp",
    ],
    visibility = ["//visibility:private"],
    linkopts = DEFAULT_EMSCRIPTEN_LINKOPTS + NODE_LINKOPTS,
    deps = [
        "//private_set_intersection/cpp:package",
        "@com_google_absl//absl/status:statusor",
        "//private_set_intersection/cpp:psi_client",
        "//private_set_intersection/cpp:psi_server",
    ],
)
cc_binary(
    name = "combined_wasm_web.js",
    srcs = [
        "utils.h",
        "client.cpp",
        "data_structure.cpp",
        "package.cpp",
        "server.cpp",
    ],
    visibility = ["//visibility:private"],
    linkopts = DEFAULT_EMSCRIPTEN_LINKOPTS + WEB_LINKOPTS,
    deps = [
        "//private_set_intersection/cpp:package",
        "@com_google_absl//absl/status:statusor",
        "//private_set_intersection/cpp:psi_client",
        "//private_set_intersection/cpp:psi_server",
    ],
)
cc_binary(
    name = "combined_wasm_worker.js",
    srcs = [
        "utils.h",
        "client.cpp",
        "data_structure.cpp",
        "package.cpp",
        "server.cpp",
    ],
    visibility = ["//visibility:private"],
    linkopts = DEFAULT_EMSCRIPTEN_LINKOPTS + WORKER_LINKOPTS,
    deps = [
        "//private_set_intersection/cpp:package",
        "@com_google_absl//absl/status:statusor",
        "//private_set_intersection/cpp:psi_client",
        "//private_set_intersection/cpp:psi_server",
    ],
)
wasm_cc_binary(
    name = "psi_combined_wasm_node.js",
    cc_target = ":combined_wasm_node.js",
)
wasm_cc_binary(
    name = "psi_combined_wasm_web.js",
    cc_target = ":combined_wasm_web.js",
)
wasm_cc_binary(
    name = "psi_combined_wasm_worker.js",
    cc_target = ":combined_wasm_worker.js",
)

# Benchmark
cc_binary(
    name = "benchmark_wasm.js",
    srcs = ["//private_set_intersection/cpp:psi_benchmark.cpp"],
    visibility = ["//visibility:private"],
    linkopts = DEFAULT_EMSCRIPTEN_LINKOPTS + NODE_LINKOPTS,
    deps = [
        "//private_set_intersection/cpp:psi_client",
        "//private_set_intersection/cpp:psi_server",
        "@com_google_absl//absl/strings",
        "@com_google_benchmark//:benchmark_main",
    ],
)

wasm_cc_binary(
    name = "psi_benchmark_wasm.js",
    cc_target = ":benchmark_wasm.js",
)
