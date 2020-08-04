extern crate rust_psi;

use rust_psi::*;

use std::collections::HashSet;

#[test]
fn test_client_server() {
    for &reveal in &[false, true] {
        let client = client::PsiClient::create_with_new_key(reveal).unwrap();
        let server = server::PsiServer::create_with_new_key(reveal).unwrap();

        let num_client_elements = 1000;
        let num_server_elements = 10000;
        let client_elements = generate_elements(num_client_elements, 1);
        let server_elements = generate_elements(num_server_elements, 2);
        let fpr = 0.000000001f64;

        let setup = server.create_setup_message(fpr, num_client_elements, &server_elements).unwrap();

        // setup message contains server elements
        // setup message is sent to the client

        let request = client.create_request(&client_elements).unwrap();

        // request contains client elements encrypted with client secret key
        // request is sent to the server

        let response = server.process_request(&request).unwrap();

        // response contains client elements encrypted with both client and server secret keys
        // response is sent to the client

        if reveal {
            let intersection = client.get_intersection(&setup, &response).unwrap();
            let set = HashSet::from_iter(intersection.into_iter());

            for i in 0..(num_client_elements / 2) as i64 {
                assert!(set.contains(i * 2));
            }
        } else {
            let intersection_size = client.get_intersection_size(&setup, &response).unwrap();

            assert!(intersection_size >= (num_client_elements / 2));
            assert!((intersection_size as f64) < ((num_client_elements as f64) / 2.0 * 1.1));
        }
    }
}

fn generate_elements(n: usize, m: usize) -> Vec<String> {
    (0..n).map(|i| format!("Element {}", i * m)).collect()
}
