load("@npm//@bazel/typescript:index.bzl", "ts_project")

package(default_visibility = ["//visibility:public"])

# TODO: Convert manual tsc to bazel
ts_project(
    name = "lib",
    tsconfig = "//:tsconfig.json",
    srcs = glob(["*.ts"]),
    deps = [
        "@npm//@types/google-protobuf",
        "@npm//google-protobuf",
        "@npm//@grpc/grpc-js",
    ],
)