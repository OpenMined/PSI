load("@io_bazel_rules_go//go:def.bzl", "go_library", "go_test")

go_library(
    name = "server",
    srcs = ["server.go"],
    cdeps = ["//psi_cardinality/c:psi_cardinality_server"],
    cgo = True,
    importpath = "github.com/openmined/psi-cardinality/server",
    visibility = ["//visibility:public"],
)

go_test(
    name = "server_test",
    srcs = ["server_test.go"],
    embed = [":server"],
    deps = ["//psi_cardinality/go/client"],
)
