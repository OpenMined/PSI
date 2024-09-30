load("@rules_go//go:def.bzl", "go_library", "go_test")

go_library(
    name = "server",
    srcs = ["server.go"],
    cdeps = ["//private_set_intersection/c:c_psi_server"],
    cgo = True,
    importpath = "github.com/openmined/psi/server",
    visibility = ["//visibility:public"],
    deps = [
        "//private_set_intersection/go/datastructure",
        "//private_set_intersection/go/version",
        "//private_set_intersection/proto:psi_go_proto",
        "@org_golang_google_protobuf//proto:go_default_library",
    ],
)

go_test(
    name = "server_test",
    srcs = ["server_test.go"],
    embed = [":server"],
    deps = [
        "//private_set_intersection/go/client",
        "//private_set_intersection/go/datastructure",
        "//private_set_intersection/proto:psi_go_proto",
    ],
)
