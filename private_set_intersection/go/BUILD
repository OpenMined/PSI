load("@bazel_gazelle//:def.bzl", "gazelle")
load("@io_bazel_rules_go//go:def.bzl", "go_test")

# gazelle:prefix github.com/openmined/psi
gazelle(name = "gazelle")

go_test(
    name = "integration_test",
    srcs = ["integration_test.go"],
    importpath = "github.com/openmined/psi",
    deps = [
        "//private_set_intersection/go/client",
        "//private_set_intersection/go/server",
        "//private_set_intersection/go/version",
        "//private_set_intersection/proto:psi_go_proto",
        "@org_golang_google_protobuf//proto:go_default_library",
    ],
)
