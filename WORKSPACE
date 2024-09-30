workspace(
    name = "org_openmined_psi",
)

# Emsdk
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "emsdk",
    remote = "https://github.com/emscripten-core/emsdk.git",
    strip_prefix = "bazel",
    tag = "3.1.67",
)

load("@emsdk//:deps.bzl", emsdk_deps = "deps")

emsdk_deps()

load("@emsdk//:emscripten_deps.bzl", emsdk_emscripten_deps = "emscripten_deps")

emsdk_emscripten_deps(emscripten_version = "3.1.67")

load("@emsdk//:toolchains.bzl", "register_emscripten_toolchains")

register_emscripten_toolchains()
