extern crate bindgen;

use std::env;
use std::path::PathBuf;

fn main() {
    println!("cargo:rustc-link-lib=bz2");

    let bindings = bindgen::Builder::default()
        .header("wrapper.h")
        .clang_arg("-I../cpp")
        .clang_arg("-I/home/username/repos/abseil-cpp")
        .clang_arg("-I/usr/include/c++/9")
        .clang_arg("-I/snap/gnome-3-34-1804/36/usr/include/x86_64-linux-gnu/c++/6")
        .generate()
        .expect("unable to generate bindings");

    // Write the bindings to the $OUT_DIR/bindings.rs file.
    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings!");
}
