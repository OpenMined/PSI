package(default_visibility = ["//visibility:public"])

DEFAULT_EMSCRIPTEN_LINKOPTS = [
    "-flto",                            # Specify lto (has to be set on for compiler as well)
    "--bind",                           # Compiles the source code using the Embind bindings to connect C/C++ and JavaScript
    "--closure 1",                      # Run the closure compiler
    "-s MALLOC=emmalloc",               # Switch to using the much smaller implementation
    "-s ALLOW_MEMORY_GROWTH=1",         # Allow both asm.js and wasm to increase memory dynamically
    "-s USE_PTHREADS=0",                # Ensure we disable pthreads
    "-s ASSERTIONS=0",                  # Turn off runtime assertions
    "-s EXPORT_ES6=1",                  # Export as es6 module, used for rollup
    "-s MODULARIZE=1",                  # Allows us to manually invoke the initializatio of wasm
    "-s EXPORT_NAME=createModule",      # Not used, but good to specify
    "-s USE_ES6_IMPORT_META=0",         # Disable loading from import meta since we use rollup
    "-s SINGLE_FILE=1",                 # Pack all webassembly into base64
    "-s WASM_BIGINT=0",                 # Disable WebAssembly integration with JavaScript BigInt as it isn't used
    "-s DISABLE_EXCEPTION_CATCHING=1",  # Disable all exception catching
    "-s NODEJS_CATCH_EXIT=0"            # We don't have a 'main' so disable exit() catching
]

BENCHMARK_LINKOPTS = [
    "-s ERROR_ON_UNDEFINED_SYMBOLS=0",  # Ignore POSIX popen
]

WASM_LINKOPTS = [
    "-s WASM=1",                        # Specify wasm ouptut
]

ASMJS_LINKOPTS = [
    "-s WASM=0",                        # Specify asm.js ouptut
]

cc_library(
    name = "utils",
    hdrs = ["bindings/utils.h"],
    visibility = ["//visibility:private"],
    deps = [
        "@private_join_and_compute//util:status",
    ],
)

cc_library(
    name = "package",
    srcs = ["bindings/package.cpp"],
    visibility = ["//visibility:private"],
    deps = [
        "//private_set_intersection/cpp:package",
    ],
)

cc_binary(
    name = "psi_combined_wasm.js",
    srcs = [
        "bindings/client.cpp",
        "bindings/package.cpp",
        "bindings/server.cpp",
    ],
    linkopts = DEFAULT_EMSCRIPTEN_LINKOPTS + [
        "-s WASM=1",
    ],
    deps = [
        ":package",
        ":utils",
        "//private_set_intersection/cpp:psi_client",
        "//private_set_intersection/cpp:psi_server",
    ],
)

cc_binary(
    name = "psi_combined_js.js",
    srcs = [
        "bindings/client.cpp",
        "bindings/package.cpp",
        "bindings/server.cpp",
    ],
    linkopts = DEFAULT_EMSCRIPTEN_LINKOPTS + ASMJS_LINKOPTS,
    deps = [
        ":package",
        ":utils",
        "//private_set_intersection/cpp:psi_client",
        "//private_set_intersection/cpp:psi_server",
    ],
)

cc_binary(
    name = "psi_client_wasm.js",
    srcs = [
        "bindings/client.cpp",
        "bindings/package.cpp",
    ],
    linkopts = DEFAULT_EMSCRIPTEN_LINKOPTS + WASM_LINKOPTS,
    deps = [
        ":package",
        ":utils",
        "//private_set_intersection/cpp:psi_client",
    ],
)

cc_binary(
    name = "psi_client_js.js",
    srcs = [
        "bindings/client.cpp",
        "bindings/package.cpp",
    ],
    linkopts = DEFAULT_EMSCRIPTEN_LINKOPTS + ASMJS_LINKOPTS,
    deps = [
        ":package",
        ":utils",
        "//private_set_intersection/cpp:psi_client",
    ],
)

cc_binary(
    name = "psi_server_wasm.js",
    srcs = [
        "bindings/package.cpp",
        "bindings/server.cpp",
    ],
    linkopts = DEFAULT_EMSCRIPTEN_LINKOPTS + WASM_LINKOPTS,
    deps = [
        ":package",
        ":utils",
        "//private_set_intersection/cpp:psi_server",
    ],
)

cc_binary(
    name = "psi_server_js.js",
    srcs = [
        "bindings/package.cpp",
        "bindings/server.cpp",
    ],
    linkopts = DEFAULT_EMSCRIPTEN_LINKOPTS + ASMJS_LINKOPTS,
    deps = [
        ":package",
        ":utils",
        "//private_set_intersection/cpp:psi_server",
    ],
)

cc_binary(
    name = "psi_benchmark_wasm.js",
    srcs = ["//private_set_intersection/cpp:psi_benchmark.cpp"],
    linkopts = DEFAULT_EMSCRIPTEN_LINKOPTS + WASM_LINKOPTS + BENCHMARK_LINKOPTS,
    deps = [
        "//private_set_intersection/cpp:psi_client",
        "//private_set_intersection/cpp:psi_server",
        "@com_google_absl//absl/strings",
        "@com_google_benchmark//:benchmark_main",
    ],
)

cc_binary(
    name = "psi_benchmark_js.js",
    srcs = ["//private_set_intersection/cpp:psi_benchmark.cpp"],
    linkopts = DEFAULT_EMSCRIPTEN_LINKOPTS + ASMJS_LINKOPTS + BENCHMARK_LINKOPTS,
    deps = [
        "//private_set_intersection/cpp:psi_client",
        "//private_set_intersection/cpp:psi_server",
        "@com_google_absl//absl/strings",
        "@com_google_benchmark//:benchmark_main",
    ],
)
