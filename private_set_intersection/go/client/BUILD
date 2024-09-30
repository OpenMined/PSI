load("@rules_go//go:def.bzl", "go_library", "go_test")

go_library(
    name = "client",
    srcs = ["client.go"],
    cdeps = ["//private_set_intersection/c:c_psi_client"],
    cgo = True,
    importpath = "github.com/openmined/psi/client",
    visibility = ["//visibility:public"],
    deps = [
        "//private_set_intersection/go/version",
        "//private_set_intersection/proto:psi_go_proto",
        "@org_golang_google_protobuf//proto:go_default_library",
    ],
)

go_test(
    name = "client_test",
    srcs = ["client_test.go"],
    embed = [":client"],
    deps = ["//private_set_intersection/proto:psi_go_proto"],
)
