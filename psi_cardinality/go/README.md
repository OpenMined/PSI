# PSI Cardinality - Go [![Go Report Card](https://goreportcard.com/badge/github.com/OpenMined/PSI)](https://goreportcard.com/report/github.com/OpenMined/PSI)

Private Set Intersection Cardinality protocol based on ECDH and Bloom Filters.


## PSI client [![Documentation](https://img.shields.io/badge/godoc-reference-blue.svg)](https://pkg.go.dev/github.com/OpenMined/PSI/psi_cardinality/go/client)
```
import "github.com/openmined/psi-cardinality/client"
```

## PSI server [![Documentation](https://img.shields.io/badge/godoc-reference-blue.svg)](https://pkg.go.dev/github.com/OpenMined/PSI/psi_cardinality/go/server)
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

## Integration

* Add Bazel depends to your WORKSPACE, as indicated in the [Using the Library](../../README.md) section.
* Add the server or the client as to your deps in the BUILD file


```
go_library(
    name = "go_default_library",
    srcs = ["main.go"],
    importpath = "github.com/openmined/psi",
    deps = [
            "@org_openmined_psi_cardinality//psi_cardinality/go/server:server",
            "@org_openmined_psi_cardinality//psi_cardinality/go/client:client",
            ],
)
```


* Import and use it



```go
package main
import (
    "fmt"
    "github.com/openmined/psi-cardinality/server"
    "github.com/openmined/psi-cardinality/client"
)

func main(){
    psiServer, err := server.CreateWithNewKey()
    if err == nil {
        fmt.Println("loaded server")
        psiServer.Destroy()
    }

    psiClient, err := client.Create()
    if err == nil  {
        fmt.Println("loaded client")
        psiClient.Destroy()
    }
}
```

