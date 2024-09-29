load("@rules_go//go:def.bzl", "go_library", "go_test")

go_library(
    name = "version",
    srcs = ["version.go"],
    cdeps = ["//private_set_intersection/c:c_package"],
    cgo = True,
    importpath = "github.com/openmined/psi/version",
    visibility = ["//visibility:public"],
)

go_test(
    name = "version_test",
    srcs = ["version_test.go"],
    embed = [":version"],
)
