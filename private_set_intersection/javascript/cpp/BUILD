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

# psi server + client build
cc_binary(
    name = "wasm_node.js",
    srcs = [
        "client.cpp",
        "data_structure.cpp",
        "package.cpp",
        "server.cpp",
        "utils.h",
    ],
    linkopts = DEFAULT_EMSCRIPTEN_LINKOPTS + NODE_LINKOPTS,
    visibility = ["//visibility:private"],
    deps = [
        "//private_set_intersection/cpp:package",
        "//private_set_intersection/cpp:psi_client",
        "//private_set_intersection/cpp:psi_server",
    ],
)

cc_binary(
    name = "wasm_web.js",
    srcs = [
        "client.cpp",
        "data_structure.cpp",
        "package.cpp",
        "server.cpp",
        "utils.h",
    ],
    linkopts = DEFAULT_EMSCRIPTEN_LINKOPTS + WEB_LINKOPTS,
    visibility = ["//visibility:private"],
    deps = [
        "//private_set_intersection/cpp:package",
        "//private_set_intersection/cpp:psi_client",
        "//private_set_intersection/cpp:psi_server",
    ],
)

cc_binary(
    name = "wasm_worker.js",
    srcs = [
        "client.cpp",
        "data_structure.cpp",
        "package.cpp",
        "server.cpp",
        "utils.h",
    ],
    linkopts = DEFAULT_EMSCRIPTEN_LINKOPTS + WORKER_LINKOPTS,
    visibility = ["//visibility:private"],
    deps = [
        "//private_set_intersection/cpp:package",
        "//private_set_intersection/cpp:psi_client",
        "//private_set_intersection/cpp:psi_server",
    ],
)

wasm_cc_binary(
    name = "psi_wasm_node.js",
    cc_target = ":wasm_node.js",
)

wasm_cc_binary(
    name = "psi_wasm_web.js",
    cc_target = ":wasm_web.js",
)

wasm_cc_binary(
    name = "psi_wasm_worker.js",
    cc_target = ":wasm_worker.js",
)

# Benchmark
cc_binary(
    name = "benchmark_wasm.js",
    srcs = ["//private_set_intersection/cpp:psi_benchmark.cpp"],
    linkopts = DEFAULT_EMSCRIPTEN_LINKOPTS + NODE_LINKOPTS,
    visibility = ["//visibility:private"],
    deps = [
        "//private_set_intersection/cpp:psi_client",
        "//private_set_intersection/cpp:psi_server",
        "@google_benchmark//:benchmark_main",
    ],
)

wasm_cc_binary(
    name = "psi_benchmark_wasm.js",
    cc_target = ":benchmark_wasm.js",
)
