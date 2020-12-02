echo on

set GO111MODULE=off 
bazel test --test_output=all //private_set_intersection/go/...
