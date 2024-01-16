workspace(
    name = "org_openmined_psi",
)

load("@org_openmined_psi//private_set_intersection:preload.bzl", "psi_preload")

psi_preload()

load("@org_openmined_psi//private_set_intersection:deps.bzl", "psi_deps")

psi_deps()

load("@rules_python//python:pip.bzl", "pip_parse")

# Finish python setup
pip_parse(
    name = "pip_deps",
    # Generated via pip-compile requirements.in
    requirements_lock = "@org_openmined_psi//private_set_intersection/python:requirements.txt",
)

load("@pip_deps//:requirements.bzl", "install_deps")

install_deps()

# Finish node setup
load("@build_bazel_rules_nodejs//:index.bzl", "node_repositories", "npm_install")

node_repositories()

npm_install(
    name = "npm",
    package_json = "//:package.json",
    package_lock_json = "//:package-lock.json",
)

# Finish emsdk setup
load("@emsdk//:emscripten_deps.bzl", emsdk_emscripten_deps = "emscripten_deps")

emsdk_emscripten_deps()
