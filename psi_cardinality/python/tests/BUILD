load("@org_openmined_psi_python_deps//:requirements.bzl", "requirement")

py_test(
    name = "tests",
    srcs = ["tests.py"],
    main = "tests.py",
    deps = [
        "//psi_cardinality/python:psi_cardinality",
        requirement("pytest"),
    ],
    python_version = "PY3",
    srcs_version = "PY3",
)
py_test(
    name = "benchmarks",
    srcs = ["benchmarks.py"],
    main = "benchmarks.py",
    deps = [
        "//psi_cardinality/python:psi_cardinality",
        requirement("pytest"),
        requirement("pytest-benchmark"),
    ],
    python_version = "PY3",
    srcs_version = "PY3",
)
