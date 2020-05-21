load("@io_bazel_rules_go//go:def.bzl", "go_library", "go_test")

go_library(
    name = "version",
    srcs = ["version.go"],
    cdeps = ["//psi_cardinality/c:package"],
    cgo = True,
    importpath = "github.com/openmined/psi-cardinality/version",
    visibility = ["//visibility:public"],
)

go_test(
    name = "version_test",
    srcs = ["version_test.go"],
    embed = [":version"],
)
