workspace(name = "com_github_openmined_psi_cardinality")

load("@com_github_openmined_psi_cardinality//src/cpp:preload.bzl", "psi_cardinality_preload")

psi_cardinality_preload()

load("@com_github_openmined_psi_cardinality//src/cpp:deps.bzl", "psi_cardinality_deps")

psi_cardinality_deps()

load("//src/javascript/toolchain:cc_toolchain_config.bzl", "emsdk_configure")

# Make all files under submodules/emsdk/* visible to the toolchain. The files are
# available as external/emsdk/emsdk/*
emsdk_configure(name = "emsdk")