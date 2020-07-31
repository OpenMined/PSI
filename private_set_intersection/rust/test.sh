cd ../..
bazel build //private_set_intersection/c/...
export BAZEL_BIN_PATH=`pwd`
cd private_set_intersection/rust
# Specify link arguments for C++ files that share the same names as their C counterparts, which causes Cargo to have trouble linking
RUSTFLAGS="-C link-args=-Wl,$BAZEL_BIN_PATH/bazel-bin/private_set_intersection/cpp/libpsi_server.a -C link-args=-Wl,$BAZEL_BIN_PATH/bazel-bin/private_set_intersection/cpp/libpsi_client.a" cargo test
