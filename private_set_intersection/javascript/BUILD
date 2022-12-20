load("@npm//@bazel/typescript:index.bzl", "ts_project")

package(default_visibility = ["//visibility:public"])

# TODO: This is not used today, but eventually we should convert the manual
# compilation steps to pure bazel workflows.
ts_project(
    name = "lib",
    srcs = glob(["*.ts"]),
    tsconfig = "//:tsconfig.json",
    deps = [
        "@npm//@grpc/grpc-js",
        "@npm//@types/google-protobuf",
        "@npm//google-protobuf",
    ],
)
