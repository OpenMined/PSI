cc_binary(
    name = "psi_cardinality_client_wasm.js",
    srcs = ["src/client_bindings.cpp"],
    linkopts = [
        "-flto",
        "--bind",
        "-s WASM=1",
        "-s ALLOW_MEMORY_GROWTH=1",
        "-s USE_PTHREADS=0",
        "-s ASSERTIONS=0",
        "-s EXPORT_ES6=0",
        "-s MODULARIZE=1",
        "-s USE_ES6_IMPORT_META=0",
        "-s SINGLE_FILE=1",
        "-s ERROR_ON_UNDEFINED_SYMBOLS=0",  # Ignore POSIX popen
        "--closure 1",
    ],
    deps = [
        "//psi_cardinality/cpp:psi_cardinality_client",
    ],
)

cc_binary(
    name = "psi_cardinality_client_js.js",
    srcs = ["src/client_bindings.cpp"],
    linkopts = [
        "-flto",
        "--bind",
        "-s WASM=0",
        "-s ALLOW_MEMORY_GROWTH=1",
        "-s USE_PTHREADS=0",
        "-s ASSERTIONS=0",
        "-s EXPORT_ES6=0",
        "-s MODULARIZE=1",
        "-s USE_ES6_IMPORT_META=0",
        "-s SINGLE_FILE=1",
        "-s ERROR_ON_UNDEFINED_SYMBOLS=0",  # Ignore POSIX popen
        "--closure 0",
    ],
    deps = [
        "//psi_cardinality/cpp:psi_cardinality_client",
    ],
)

cc_binary(
    name = "psi_cardinality_server_wasm.js",
    srcs = ["src/server_bindings.cpp"],
    linkopts = [
        "-flto",
        "--bind",
        "-s WASM=1",
        "-s ALLOW_MEMORY_GROWTH=1",
        "-s USE_PTHREADS=0",
        "-s ASSERTIONS=0",
        "-s EXPORT_ES6=0",
        "-s MODULARIZE=1",
        "-s USE_ES6_IMPORT_META=0",
        "-s SINGLE_FILE=1",
        "-s ERROR_ON_UNDEFINED_SYMBOLS=0",  # Ignore POSIX popen
        "--closure 1",
    ],
    deps = [
        "//psi_cardinality/cpp:psi_cardinality_server",
    ],
)

cc_binary(
    name = "psi_cardinality_server_js.js",
    srcs = ["src/server_bindings.cpp"],
    linkopts = [
        "-flto",
        "--bind",
        "-s WASM=0",
        "-s ALLOW_MEMORY_GROWTH=1",
        "-s USE_PTHREADS=0",
        "-s ASSERTIONS=0",
        "-s EXPORT_ES6=0",
        "-s MODULARIZE=1",
        "-s USE_ES6_IMPORT_META=0",
        "-s SINGLE_FILE=1",
        "-s ERROR_ON_UNDEFINED_SYMBOLS=0",  # Ignore POSIX popen
        "--closure 0",
    ],
    deps = [
        "//psi_cardinality/cpp:psi_cardinality_server",
    ],
)

cc_binary(
    name = "psi_cardinality_benchmark_wasm.js",
    srcs = ["//psi_cardinality/cpp:psi_cardinality_benchmark.cpp"],
    linkopts = [
        "-flto",
        "-s WASM=1",
        "-s ALLOW_MEMORY_GROWTH=1",
        "-s USE_PTHREADS=0",
        "-s ASSERTIONS=0",
        "-s EXPORT_ES6=0",
        "-s MODULARIZE=1",
        "-s USE_ES6_IMPORT_META=0",
        "-s SINGLE_FILE=1",
        "-s EXIT_RUNTIME=0",
        "-s ERROR_ON_UNDEFINED_SYMBOLS=0",  # Ignore POSIX popen
        "--closure 1",
    ],
    deps = [
        "//psi_cardinality/cpp:psi_cardinality_client",
        "//psi_cardinality/cpp:psi_cardinality_server",
        "@com_google_absl//absl/strings",
        "@com_google_benchmark//:benchmark_main",
    ],
)

cc_binary(
    name = "psi_cardinality_benchmark_js.js",
    srcs = ["//psi_cardinality/cpp:psi_cardinality_benchmark.cpp"],
    linkopts = [
        "-flto",
        "-s WASM=0",
        "-s ALLOW_MEMORY_GROWTH=1",
        "-s USE_PTHREADS=0",
        "-s ASSERTIONS=0",
        "-s EXPORT_ES6=0",
        "-s MODULARIZE=1",
        "-s USE_ES6_IMPORT_META=0",
        "-s SINGLE_FILE=1",
        "-s EXIT_RUNTIME=0",
        "-s ERROR_ON_UNDEFINED_SYMBOLS=0",  # Ignore POSIX popen
        "--closure 0",
    ],
    deps = [
        "//psi_cardinality/cpp:psi_cardinality_client",
        "//psi_cardinality/cpp:psi_cardinality_server",
        "@com_google_absl//absl/strings",
        "@com_google_benchmark//:benchmark_main",
    ],
)
