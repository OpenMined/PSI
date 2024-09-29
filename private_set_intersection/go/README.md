# Private Set Intersection - Go [![Go Report Card](https://goreportcard.com/badge/github.com/OpenMined/PSI)](https://goreportcard.com/report/github.com/OpenMined/PSI)

Private Set Intersection protocol based on ECDH, Bloom Filters, and Golomb
Compressed Sets.

## PSI client [![Documentation](https://img.shields.io/badge/godoc-reference-blue.svg)](https://pkg.go.dev/github.com/OpenMined/PSI/private_set_intersection/go/client)

```
import "github.com/openmined/psi/client"
```

## PSI server [![Documentation](https://img.shields.io/badge/godoc-reference-blue.svg)](https://pkg.go.dev/github.com/OpenMined/PSI/private_set_intersection/go/server)

```
import "github.com/openmined/psi/server"
```

## Tests

```
bazel test --test_output=all //private_set_intersection/go/...
```

## Benchmarks

```
bazel test -c opt --test_output=all --test_arg=-test.bench=. //private_set_intersection/go/...
```

## Integration

- Add Bazel depends to your WORKSPACE, as indicated in the
  [Usage](https://github.com/OpenMined/PSI#Usage) section.
- Add the server or the client to your deps in the BUILD file

```
go_library(
    name = "go_default_library",
    srcs = ["main.go"],
    importpath = "github.com/openmined/psi",
    deps = [
            "@org_openmined_psi//private_set_intersection/go/server",
            "@org_openmined_psi//private_set_intersection/go/client",
            ],
)
```

- Import and use the library

```go
package main
import (
    "fmt"
    "github.com/openmined/psi/server"
    "github.com/openmined/psi/client"
)

func main(){
    revealIntersection := false
    psiServer, err := server.CreateWithNewKey(revealIntersection)
    if err == nil {
        fmt.Println("server loaded")
        psiServer.Destroy()
    }

    psiClient, err := client.CreateWithNewKey(revealIntersection)
    if err == nil  {
        fmt.Println("client loaded")
        psiClient.Destroy()
    }
}
```
