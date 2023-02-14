package benchmark

import (
	"fmt"
	"testing"

	psi_client "github.com/openmined/psi/client"
	psi_ds "github.com/openmined/psi/datastructure"
	psi_server "github.com/openmined/psi/server"
	"google.golang.org/protobuf/proto"
)

var paramsTable = []struct {
	revealIntersection bool
	ds                 psi_ds.DataStructure
}{
	{revealIntersection: true, ds: psi_ds.Raw},
	{revealIntersection: true, ds: psi_ds.Gcs},
	{revealIntersection: true, ds: psi_ds.BloomFilter},
}

var inputsTable = []struct {
	fpr             float64
	numClientInputs int
}{
	{fpr: 0.001, numClientInputs: 10},
	{fpr: 0.001, numClientInputs: 100},
}

const numServerInputs = 1000

func BenchmarkServerSetup(b *testing.B) {
	for _, pt := range paramsTable {
		server, err := psi_server.CreateWithNewKey(pt.revealIntersection)
		if err != nil || server == nil {
			b.Errorf("failed to get server")
		}
		for _, it := range inputsTable {
			b.Run(fmt.Sprintf("ds_%s_fpr_%f_inputs_%d", pt.ds, it.fpr, it.numClientInputs), func(b *testing.B) {
				b.ReportAllocs()
				total := 0
				serverInputs := []string{}
				for i := 0; i < numServerInputs; i++ {
					serverInputs = append(serverInputs, "Element "+string(i))
				}

				for i := 0; i < b.N; i++ {
					_, err := server.CreateSetupMessage(it.fpr, int64(10000), serverInputs, pt.ds)
					if err != nil {
						b.Errorf("failed to create setup msg %v", err)
					}
					total += numServerInputs
				}

				b.ReportMetric(float64(total), "ElementsProcessed")
			})
		}
	}
}

func BenchmarkClientRequest(b *testing.B) {
	for _, pt := range paramsTable {
		client, err := psi_client.CreateWithNewKey(pt.revealIntersection)
		if err != nil {
			b.Errorf("failed to get client")
		}

		for _, it := range inputsTable {
			b.Run(fmt.Sprintf("ds_%s_fpr_%f_inputs_%d", pt.ds, it.fpr, it.numClientInputs), func(b *testing.B) {
				b.ReportAllocs()
				total := 0

				clientInputs := []string{}
				for i := 0; i < it.numClientInputs; i++ {
					clientInputs = append(clientInputs, "Element "+string(i))
				}

				for n := 0; n < b.N; n++ {
					request, err := client.CreateRequest(clientInputs)
					if err != nil {
						b.Errorf("failed to create request %v", err)
					}
					total += it.numClientInputs
					b.ReportMetric(float64(proto.Size(request)), "RequestSize")
				}
				b.ReportMetric(float64(total), "ElementsProcessed")
			})
		}
	}
}

func BenchmarkProcessRequest(b *testing.B) {
	for _, pt := range paramsTable {
		client, err := psi_client.CreateWithNewKey(pt.revealIntersection)
		if err != nil {
			b.Errorf("failed to get client")
		}
		server, err := psi_server.CreateWithNewKey(pt.revealIntersection)
		if err != nil {
			b.Errorf("failed to get server")
		}

		for _, it := range inputsTable {
			b.Run(fmt.Sprintf("ds_%s_fpr_%f_inputs_%d", pt.ds, it.fpr, it.numClientInputs), func(b *testing.B) {
				b.ReportAllocs()
				total := 0

				clientInputs := []string{}
				for i := 0; i < it.numClientInputs; i++ {
					clientInputs = append(clientInputs, "Element "+string(i))
				}

				request, err := client.CreateRequest(clientInputs)
				if err != nil {
					b.Errorf("failed to create request %v", err)
				}

				for n := 0; n < b.N; n++ {
					serverResp, err := server.ProcessRequest(request)
					if err != nil {
						b.Errorf("failed to process request %v", err)
					}
					total += it.numClientInputs
					b.ReportMetric(float64(proto.Size(serverResp)), "ResponseSize")
				}
				b.ReportMetric(float64(total), "ElementsProcessed")
			})
		}
	}
}

func BenchmarkIntersection(b *testing.B) {
	for _, pt := range paramsTable {
		client, err := psi_client.CreateWithNewKey(pt.revealIntersection)
		if err != nil {
			b.Errorf("failed to get client")
		}
		server, err := psi_server.CreateWithNewKey(pt.revealIntersection)
		if err != nil {
			b.Errorf("failed to get server")
		}

		for _, it := range inputsTable {
			b.Run(fmt.Sprintf("ds_%s_fpr_%f_inputs_%d", pt.ds, it.fpr, it.numClientInputs), func(b *testing.B) {
				b.ReportAllocs()
				total := 0

				clientInputs := []string{}
				for i := 0; i < it.numClientInputs; i++ {
					clientInputs = append(clientInputs, "Element "+string(i))
				}
				serverInputs := []string{}
				for i := 0; i < numServerInputs; i++ {
					serverInputs = append(serverInputs, "Element "+string(i*2))
				}

				setup, err := server.CreateSetupMessage(it.fpr, int64(10000), serverInputs, pt.ds)
				if err != nil {
					b.Errorf("failed to create setup msg %v", err)
				}

				request, err := client.CreateRequest(clientInputs)
				if err != nil {
					b.Errorf("failed to create request %v", err)
				}
				serverResp, err := server.ProcessRequest(request)
				if err != nil {
					b.Errorf("failed to process request %v", err)
				}

				for n := 0; n < b.N; n++ {
					intersection, err := client.GetIntersection(setup, serverResp)
					if err != nil {
						b.Errorf("failed to compute intersection %v", err)
					}
					total += len(intersection)
				}
				b.ReportMetric(float64(total), "ElementsProcessed")
			})
		}
	}
}
