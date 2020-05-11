workspace(name = "org_openmined_psi_cardinality")

load("//psi_cardinality:preload.bzl", "psi_cardinality_preload")

psi_cardinality_preload()

load("//psi_cardinality:deps.bzl", "psi_cardinality_deps")

psi_cardinality_deps()

load("@pybind11_bazel//:python_configure.bzl", "python_configure")
python_configure(name = "local_config_python")
