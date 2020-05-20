load("@io_bazel_rules_go//go:def.bzl", "go_library", "go_test")

go_library(
    name = "client",
    srcs = ["client.go"],
    cdeps = ["//psi_cardinality/c:psi_cardinality_client"],
    cgo = True,
    importpath = "github.com/openmined/psi-cardinality/client",
    visibility = ["//visibility:public"],
    deps = ["//psi_cardinality/go/version"],
)

go_test(
    name = "client_test",
    srcs = ["client_test.go"],
    embed = [":client"],
    deps = ["//psi_cardinality/go/server"],
)
