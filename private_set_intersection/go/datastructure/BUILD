load("@rules_go//go:def.bzl", "go_library")

go_library(
    name = "datastructure",
    srcs = ["datastructure.go"],
    cdeps = ["//private_set_intersection/cpp/datastructure"],
    cgo = True,
    importpath = "github.com/openmined/psi/datastructure",
    visibility = ["//visibility:public"],
)
