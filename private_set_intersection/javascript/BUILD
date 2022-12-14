load("@npm//@bazel/typescript:index.bzl", "ts_project")

package(default_visibility = ["//visibility:public"])

# TODO: Convert manual tsc to bazel
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
