# PSI Cardinality - Go [![Go Report Card](https://goreportcard.com/badge/github.com/openmined/psi-cardinality)](https://goreportcard.com/report/github.com/openmined/psi-cardinality)

Private Set Intersection Cardinality protocol based on ECDH and Bloom Filters.


## PSI client [![Documentation](https://img.shields.io/badge/godoc-reference-blue.svg)](https://pkg.go.dev/github.com/openmined/psi-cardinality/psi_cardinality/go/client)
```
import "github.com/openmined/psi-cardinality/client"
```

## PSI server [![Documentation](https://img.shields.io/badge/godoc-reference-blue.svg)](https://pkg.go.dev/github.com/openmined/psi-cardinality/psi_cardinality/go/server)
```
import "github.com/openmined/psi-cardinality/server"
```

## Tests
```
bazel test //psi_cardinality/go/... --test_output=all
```

## Benchmarks
```
bazel test //psi_cardinality/go/... --test_arg=-test.bench=. --test_output=all
```



