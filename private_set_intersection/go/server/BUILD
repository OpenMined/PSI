load("@io_bazel_rules_go//go:def.bzl", "go_library", "go_test")

go_library(
    name = "server",
    srcs = ["server.go"],
    cdeps = ["//private_set_intersection/c:psi_server"],
    cgo = True,
    importpath = "github.com/openmined/psi/server",
    visibility = ["//visibility:public"],
    deps = ["//private_set_intersection/go/version"],
)

go_test(
    name = "server_test",
    srcs = ["server_test.go"],
    embed = [":server"],
    deps = ["//private_set_intersection/go/client"],
)
