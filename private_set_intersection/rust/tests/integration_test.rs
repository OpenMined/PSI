use rust_psi::*;
use std::collections::HashSet;
use std::iter::FromIterator;

static CLIENT_KEY: &'static [u8] = &[
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
    26, 27, 28, 29, 30, 31,
];
static SERVER_KEY: &'static [u8] = &[
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
    27, 28, 29, 30, 31, 32,
];
const FPR: f64 = 0.01;
const NUM_CLIENT_ELEMENTS: usize = 10;
const NUM_SERVER_ELEMENTS: usize = 100;

#[test]
fn test_client_server() {
    for &reveal in &[false, true] {
        let client = client::PsiClient::create_from_key(CLIENT_KEY, reveal).unwrap();
        let server = server::PsiServer::create_from_key(SERVER_KEY, reveal).unwrap();

        let client_elements: Vec<String> = (0..NUM_CLIENT_ELEMENTS)
            .map(|i| format!("Element {}", i))
            .collect();
        let server_elements: Vec<String> = (0..NUM_SERVER_ELEMENTS)
            .map(|i| format!("Element {}", i * 2))
            .collect();

        let setup = server
            .create_setup_message(FPR, NUM_CLIENT_ELEMENTS, &server_elements)
            .unwrap();

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
            let set: HashSet<i64> = HashSet::from_iter(intersection.into_iter());

            for i in 0..NUM_CLIENT_ELEMENTS as i64 {
                assert_eq!(set.contains(&i), i % 2 == 0);
            }
        } else {
            let intersection_size = client.get_intersection_size(&setup, &response).unwrap();

            assert!(intersection_size >= (NUM_CLIENT_ELEMENTS / 2));
            assert!((intersection_size as f64) < ((NUM_CLIENT_ELEMENTS as f64) / 2.0 * 1.1));
        }
    }
}
