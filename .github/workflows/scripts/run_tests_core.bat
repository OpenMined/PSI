echo on

bazel test --test_output=all --test_timeout=900 //private_set_intersection/cpp:psi_client_test
bazel test --test_output=all --test_timeout=900 //private_set_intersection/cpp:psi_server_test
bazel test --test_output=all --test_timeout=900 //private_set_intersection/cpp:bloom_filter_test
bazel test --test_output=all --test_timeout=900 //private_set_intersection/cpp:gcs_test
bazel test --test_output=all --test_timeout=900 //private_set_intersection/cpp:golomb_test

bazel test --test_output=all --test_timeout=900 //private_set_intersection/c:psi_client_test
bazel test --test_output=all --test_timeout=900 //private_set_intersection/c:psi_server_test
bazel test --test_output=all --test_timeout=900 //private_set_intersection/c:bloom_filter_test
bazel test --test_output=all --test_timeout=900 //private_set_intersection/c:gcs_test
bazel test --test_output=all --test_timeout=900 //private_set_intersection/c:golomb_test
