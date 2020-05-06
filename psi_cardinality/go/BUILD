load("@io_bazel_rules_go//go:deps.bzl","go_download_sdk","go_rules_dependencies","go_register_toolchains")
#go_download_sdk(name = "go_sdk", goos = "linux", goarch = "amd64", version = "1.14.2")

go_rules_dependencies()
go_register_toolchains()

load("@rules_pkg//:deps.bzl", "rules_pkg_dependencies")

rules_pkg_dependencies()
