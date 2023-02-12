#[macro_use]
extern crate bencher;
use bencher::{black_box, Bencher};
use psi::*;

fn bench_client_create_request(reveal: bool, n: usize, b: &mut Bencher) {
    let mut elements = vec![];

    for i in 0..n {
        elements.push(format!("Elements {}", i));
    }

    elements = black_box(elements);

    let client = client::PsiClient::create_with_new_key(reveal).unwrap();

    b.iter(|| client.create_request(&elements).unwrap());
}

fn bench_client_create_request_no_reveal_1(b: &mut Bencher) {
    bench_client_create_request(false, 1, b);
}

fn bench_client_create_request_no_reveal_10(b: &mut Bencher) {
    bench_client_create_request(false, 10, b);
}

fn bench_client_create_request_no_reveal_100(b: &mut Bencher) {
    bench_client_create_request(false, 100, b);
}

fn bench_client_create_request_no_reveal_1000(b: &mut Bencher) {
    bench_client_create_request(false, 1000, b);
}

fn bench_client_create_request_no_reveal_10000(b: &mut Bencher) {
    bench_client_create_request(false, 10000, b);
}

fn bench_client_create_request_reveal_1(b: &mut Bencher) {
    bench_client_create_request(true, 1, b);
}

fn bench_client_create_request_reveal_10(b: &mut Bencher) {
    bench_client_create_request(true, 10, b);
}

fn bench_client_create_request_reveal_100(b: &mut Bencher) {
    bench_client_create_request(true, 100, b);
}

fn bench_client_create_request_reveal_1000(b: &mut Bencher) {
    bench_client_create_request(true, 1000, b);
}

fn bench_client_create_request_reveal_10000(b: &mut Bencher) {
    bench_client_create_request(true, 10000, b);
}

fn bench_client_process_response(reveal: bool, n: usize, b: &mut Bencher) {
    let mut elements = vec![];

    for i in 0..n {
        elements.push(format!("Elements {}", i));
    }

    elements = black_box(elements);

    let client = client::PsiClient::create_with_new_key(reveal).unwrap();
    let server = server::PsiServer::create_with_new_key(reveal).unwrap();

    let fpr = 1e-9f64;

    let setup = server
        .create_setup_message(fpr, n, &elements, None)
        .unwrap();
    let request = client.create_request(&elements).unwrap();
    let response = server.process_request(&request).unwrap();

    b.iter(|| {
        if reveal {
            let intersection = client.get_intersection(&setup, &response).unwrap();
            black_box(intersection);
        } else {
            let intersection_size = client.get_intersection_size(&setup, &response).unwrap();
            black_box(intersection_size);
        }
    });
}

fn bench_client_process_response_no_reveal_1(b: &mut Bencher) {
    bench_client_process_response(false, 1, b);
}

fn bench_client_process_response_no_reveal_10(b: &mut Bencher) {
    bench_client_process_response(false, 10, b);
}

fn bench_client_process_response_no_reveal_100(b: &mut Bencher) {
    bench_client_process_response(false, 100, b);
}

fn bench_client_process_response_no_reveal_1000(b: &mut Bencher) {
    bench_client_process_response(false, 1000, b);
}

fn bench_client_process_response_no_reveal_10000(b: &mut Bencher) {
    bench_client_process_response(false, 10000, b);
}

fn bench_client_process_response_reveal_1(b: &mut Bencher) {
    bench_client_process_response(true, 1, b);
}

fn bench_client_process_response_reveal_10(b: &mut Bencher) {
    bench_client_process_response(true, 10, b);
}

fn bench_client_process_response_reveal_100(b: &mut Bencher) {
    bench_client_process_response(true, 100, b);
}

fn bench_client_process_response_reveal_1000(b: &mut Bencher) {
    bench_client_process_response(true, 1000, b);
}

fn bench_client_process_response_reveal_10000(b: &mut Bencher) {
    bench_client_process_response(true, 10000, b);
}

fn bench_server_setup(reveal: bool, n: usize, fpr: f64, b: &mut Bencher) {
    let mut elements = vec![];

    for i in 0..n {
        elements.push(format!("Elements {}", i));
    }

    elements = black_box(elements);

    let server = server::PsiServer::create_with_new_key(reveal).unwrap();

    b.iter(|| {
        server
            .create_setup_message(fpr, 10000, &elements, None)
            .unwrap()
    });
}

const FPR3: f64 = 1e-3f64;
const FPR6: f64 = 1e-6f64;

fn bench_server_setup_no_reveal_1_fpr3(b: &mut Bencher) {
    bench_server_setup(false, 1, FPR3, b);
}

fn bench_server_setup_no_reveal_10_fpr3(b: &mut Bencher) {
    bench_server_setup(false, 10, FPR3, b);
}

fn bench_server_setup_no_reveal_100_fpr3(b: &mut Bencher) {
    bench_server_setup(false, 100, FPR3, b);
}

fn bench_server_setup_no_reveal_1000_fpr3(b: &mut Bencher) {
    bench_server_setup(false, 1000, FPR3, b);
}

fn bench_server_setup_no_reveal_10000_fpr3(b: &mut Bencher) {
    bench_server_setup(false, 10000, FPR3, b);
}

fn bench_server_setup_reveal_1_fpr3(b: &mut Bencher) {
    bench_server_setup(true, 1, FPR3, b);
}

fn bench_server_setup_reveal_10_fpr3(b: &mut Bencher) {
    bench_server_setup(true, 10, FPR3, b);
}

fn bench_server_setup_reveal_100_fpr3(b: &mut Bencher) {
    bench_server_setup(true, 100, FPR3, b);
}

fn bench_server_setup_reveal_1000_fpr3(b: &mut Bencher) {
    bench_server_setup(true, 1000, FPR3, b);
}

fn bench_server_setup_reveal_10000_fpr3(b: &mut Bencher) {
    bench_server_setup(true, 10000, FPR3, b);
}

fn bench_server_setup_no_reveal_1_fpr6(b: &mut Bencher) {
    bench_server_setup(false, 1, FPR6, b);
}

fn bench_server_setup_no_reveal_10_fpr6(b: &mut Bencher) {
    bench_server_setup(false, 10, FPR6, b);
}

fn bench_server_setup_no_reveal_100_fpr6(b: &mut Bencher) {
    bench_server_setup(false, 100, FPR6, b);
}

fn bench_server_setup_no_reveal_1000_fpr6(b: &mut Bencher) {
    bench_server_setup(false, 1000, FPR6, b);
}

fn bench_server_setup_no_reveal_10000_fpr6(b: &mut Bencher) {
    bench_server_setup(false, 10000, FPR6, b);
}

fn bench_server_setup_reveal_1_fpr6(b: &mut Bencher) {
    bench_server_setup(true, 1, FPR6, b);
}

fn bench_server_setup_reveal_10_fpr6(b: &mut Bencher) {
    bench_server_setup(true, 10, FPR6, b);
}

fn bench_server_setup_reveal_100_fpr6(b: &mut Bencher) {
    bench_server_setup(true, 100, FPR6, b);
}

fn bench_server_setup_reveal_1000_fpr6(b: &mut Bencher) {
    bench_server_setup(true, 1000, FPR6, b);
}

fn bench_server_setup_reveal_10000_fpr6(b: &mut Bencher) {
    bench_server_setup(true, 10000, FPR6, b);
}

fn bench_server_process_request(reveal: bool, n: usize, b: &mut Bencher) {
    let mut elements = vec![];

    for i in 0..n {
        elements.push(format!("Elements {}", i));
    }

    elements = black_box(elements);

    let client = client::PsiClient::create_with_new_key(reveal).unwrap();
    let server = server::PsiServer::create_with_new_key(reveal).unwrap();

    let request = client.create_request(&elements).unwrap();

    b.iter(|| server.process_request(&request).unwrap());
}

fn bench_server_process_request_no_reveal_1(b: &mut Bencher) {
    bench_server_process_request(false, 1, b);
}

fn bench_server_process_request_no_reveal_10(b: &mut Bencher) {
    bench_server_process_request(false, 10, b);
}

fn bench_server_process_request_no_reveal_100(b: &mut Bencher) {
    bench_server_process_request(false, 100, b);
}

fn bench_server_process_request_no_reveal_1000(b: &mut Bencher) {
    bench_server_process_request(false, 1000, b);
}

fn bench_server_process_request_no_reveal_10000(b: &mut Bencher) {
    bench_server_process_request(false, 10000, b);
}

fn bench_server_process_request_reveal_1(b: &mut Bencher) {
    bench_server_process_request(true, 1, b);
}

fn bench_server_process_request_reveal_10(b: &mut Bencher) {
    bench_server_process_request(true, 10, b);
}

fn bench_server_process_request_reveal_100(b: &mut Bencher) {
    bench_server_process_request(true, 100, b);
}

fn bench_server_process_request_reveal_1000(b: &mut Bencher) {
    bench_server_process_request(true, 1000, b);
}

fn bench_server_process_request_reveal_10000(b: &mut Bencher) {
    bench_server_process_request(true, 10000, b);
}

benchmark_group!(
    benches,
    bench_client_create_request_no_reveal_1,
    bench_client_create_request_no_reveal_10,
    bench_client_create_request_no_reveal_100,
    bench_client_create_request_no_reveal_1000,
    bench_client_create_request_no_reveal_10000,
    bench_client_create_request_reveal_1,
    bench_client_create_request_reveal_10,
    bench_client_create_request_reveal_100,
    bench_client_create_request_reveal_1000,
    bench_client_create_request_reveal_10000,
    bench_client_process_response_no_reveal_1,
    bench_client_process_response_no_reveal_10,
    bench_client_process_response_no_reveal_100,
    bench_client_process_response_no_reveal_1000,
    bench_client_process_response_no_reveal_10000,
    bench_client_process_response_reveal_1,
    bench_client_process_response_reveal_10,
    bench_client_process_response_reveal_100,
    bench_client_process_response_reveal_1000,
    bench_client_process_response_reveal_10000,
    bench_server_setup_no_reveal_1_fpr3,
    bench_server_setup_no_reveal_10_fpr3,
    bench_server_setup_no_reveal_100_fpr3,
    bench_server_setup_no_reveal_1000_fpr3,
    bench_server_setup_no_reveal_10000_fpr3,
    bench_server_setup_reveal_1_fpr3,
    bench_server_setup_reveal_10_fpr3,
    bench_server_setup_reveal_100_fpr3,
    bench_server_setup_reveal_1000_fpr3,
    bench_server_setup_reveal_10000_fpr3,
    bench_server_setup_no_reveal_1_fpr6,
    bench_server_setup_no_reveal_10_fpr6,
    bench_server_setup_no_reveal_100_fpr6,
    bench_server_setup_no_reveal_1000_fpr6,
    bench_server_setup_no_reveal_10000_fpr6,
    bench_server_setup_reveal_1_fpr6,
    bench_server_setup_reveal_10_fpr6,
    bench_server_setup_reveal_100_fpr6,
    bench_server_setup_reveal_1000_fpr6,
    bench_server_setup_reveal_10000_fpr6,
    bench_server_process_request_no_reveal_1,
    bench_server_process_request_no_reveal_10,
    bench_server_process_request_no_reveal_100,
    bench_server_process_request_no_reveal_1000,
    bench_server_process_request_no_reveal_10000,
    bench_server_process_request_reveal_1,
    bench_server_process_request_reveal_10,
    bench_server_process_request_reveal_100,
    bench_server_process_request_reveal_1000,
    bench_server_process_request_reveal_10000
);

benchmark_main!(benches);
