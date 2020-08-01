use protoc_rust;

fn main() {
    println!("cargo:rustc-link-lib=dylib=c++");

    let root = "../../bazel-bin";

    println!("cargo:rustc-link-search={}/private_set_intersection/c", root);
    println!("cargo:rustc-link-lib=static=psi_server");
    println!("cargo:rustc-link-lib=static=psi_client");
    println!("cargo:rustc-link-lib=static=internal_utils");

    println!("cargo:rustc-link-search={}/private_set_intersection/cpp", root);
    println!("cargo:rustc-link-lib=static=psi_server");
    println!("cargo:rustc-link-lib=static=psi_client");
    println!("cargo:rustc-link-lib=static=bloom_filter");

    println!("cargo:rustc-link-search={}/private_set_intersection/proto", root);
    println!("cargo:rustc-link-lib=static=psi_proto");

    println!("cargo:rustc-link-search={}/external/com_github_gflags_gflags", root);
    println!("cargo:rustc-link-lib=static=gflags");

    println!("cargo:rustc-link-search={}/external/com_github_glog_glog", root);
    println!("cargo:rustc-link-lib=static=glog");

    println!("cargo:rustc-link-search={}/external/private_join_and_compute/crypto", root);
    println!("cargo:rustc-link-lib=static=elgamal");
    println!("cargo:rustc-link-lib=static=openssl_init");
    println!("cargo:rustc-link-lib=static=ec_commutative_cipher");
    println!("cargo:rustc-link-lib=static=ec_util");
    println!("cargo:rustc-link-lib=static=bn_util");

    println!("cargo:rustc-link-search={}/external/private_join_and_compute/util", root);
    println!("cargo:rustc-link-lib=static=status");

    println!("cargo:rustc-link-search={}/external/zlib", root);
    println!("cargo:rustc-link-lib=static=zlib");

    println!("cargo:rustc-link-search={}/external/com_google_protobuf", root);
    println!("cargo:rustc-link-lib=static=protobuf");
    println!("cargo:rustc-link-lib=static=protobuf_lite");

    println!("cargo:rustc-link-search={}/external/boringssl", root);
    println!("cargo:rustc-link-lib=static=crypto");
    println!("cargo:rustc-link-lib=static=ssl");

    println!("cargo:rustc-link-search={}/external/com_google_absl/absl/strings", root);
    println!("cargo:rustc-link-lib=static=internal");
    println!("cargo:rustc-link-lib=static=strings");

    println!("cargo:rustc-link-search={}/external/com_google_absl/absl/numeric", root);
    println!("cargo:rustc-link-lib=static=int128");

    println!("cargo:rustc-link-search={}/external/com_google_absl/absl/base", root);
    println!("cargo:rustc-link-lib=static=dynamic_annotations");
    println!("cargo:rustc-link-lib=static=throw_delegate");
    println!("cargo:rustc-link-lib=static=log_severity");
    println!("cargo:rustc-link-lib=static=base");
    println!("cargo:rustc-link-lib=static=raw_logging_internal");
    println!("cargo:rustc-link-lib=static=spinlock_wait");

    protoc_rust::Codegen::new()
        .out_dir("src/proto")
        .include("../proto")
        .inputs(&["../proto/psi.proto"])
        .run()
        .expect("protoc");
}
