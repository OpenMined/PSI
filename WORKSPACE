workspace(name = "com_github_schoppmp_psi_cardinality")

load("@com_github_schoppmp_psi_cardinality//psi_cardinality:preload.bzl", "psi_cardinality_preload")

psi_cardinality_preload()

load("@com_github_schoppmp_psi_cardinality//psi_cardinality:deps.bzl", "psi_cardinality_deps")

psi_cardinality_deps()

load("//toolchain:cc_toolchain_config.bzl", "emsdk_configure")

# Make all files under submodules/emsdk/* visible to the toolchain. The files are
# available as external/emsdk/emsdk/*
emsdk_configure(name = "emsdk")