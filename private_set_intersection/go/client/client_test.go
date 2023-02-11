package client

import (
	psi_ds "github.com/openmined/psi/datastructure"
	psi_proto "github.com/openmined/psi/pb"
	"github.com/openmined/psi/server"
	"google.golang.org/protobuf/proto"
	"testing"
)

var result *psi_proto.Request

func benchmarkClientCreateRequest(cnt int, revealIntersection bool, b *testing.B) {
	b.ReportAllocs()
	total := 0

	client, err := CreateWithNewKey(revealIntersection)
	if err != nil || client == nil {
		b.Errorf("failed to get client")
	}
	inputs := []string{}
	for i := 0; i < cnt; i++ {
		inputs = append(inputs, "Element "+string(i))
	}

	for n := 0; n < b.N; n++ {
		request, err := client.CreateRequest(inputs)
		if err != nil {
			b.Errorf("failed to generate request")
		}

		//ugly hack for preventing compiler optimizations
		result = request

		total += cnt
		b.ReportMetric(float64(proto.Size(request)), "RequestSize")

	}
	b.ReportMetric(float64(total), "ElementsProcessed")
}

func BenchmarkClientCreateRequest1(b *testing.B)     { benchmarkClientCreateRequest(1, false, b) }
func BenchmarkClientCreateRequest10(b *testing.B)    { benchmarkClientCreateRequest(10, false, b) }
func BenchmarkClientCreateRequest100(b *testing.B)   { benchmarkClientCreateRequest(100, false, b) }
func BenchmarkClientCreateRequest1000(b *testing.B)  { benchmarkClientCreateRequest(1000, false, b) }
func BenchmarkClientCreateRequest10000(b *testing.B) { benchmarkClientCreateRequest(10000, false, b) }

func BenchmarkClientCreateRequestIntersection1(b *testing.B) {
	benchmarkClientCreateRequest(1, true, b)
}
func BenchmarkClientCreateRequestIntersection10(b *testing.B) {
	benchmarkClientCreateRequest(10, true, b)
}
func BenchmarkClientCreateRequestIntersection100(b *testing.B) {
	benchmarkClientCreateRequest(100, true, b)
}
func BenchmarkClientCreateRequestIntersection1000(b *testing.B) {
	benchmarkClientCreateRequest(1000, true, b)
}
func BenchmarkClientCreateRequestIntersection10000(b *testing.B) {
	benchmarkClientCreateRequest(10000, true, b)
}

var dummyInt64 int64

func benchmarkClientProcessResponse(cnt int, revealIntersection bool, b *testing.B) {
	b.ReportAllocs()
	total := 0

	client, err := CreateWithNewKey(revealIntersection)
	if err != nil || client == nil {
		b.Errorf("failed to get client")
	}
	server, err := server.CreateWithNewKey(revealIntersection)
	if err != nil || server == nil {
		b.Errorf("failed to get server")
	}

	fpr := 1. / (1000000)

	inputs := []string{}
	for i := 0; i < cnt; i++ {
		inputs = append(inputs, "Element "+string(i))
	}

	setup, err := server.CreateSetupMessage(fpr, int64(cnt), inputs)
	if err != nil {
		b.Errorf("failed to create setup msg %v", err)
	}
	request, err := client.CreateRequest(inputs)
	if err != nil {
		b.Errorf("failed to create request %v", err)
	}
	serverResp, err := server.ProcessRequest(request)
	if err != nil {
		b.Errorf("failed to process request %v", err)
	}

	for n := 0; n < b.N; n++ {
		if revealIntersection {
			intersection, err := client.GetIntersection(setup, serverResp)
			if err != nil {
				b.Errorf("failed to compute intersection %v", err)
			}
			dummyInt64 = int64(len(intersection))
		} else {
			intersectionCnt, err := client.GetIntersectionSize(setup, serverResp)
			if err != nil {
				b.Errorf("failed to process response %v", err)
			}
			total += cnt
			//ugly hack for preventing compiler optimizations
			dummyInt64 = intersectionCnt
		}
	}
	b.ReportMetric(float64(total), "ElementsProcessed")
}

func BenchmarkClientProcessResponse1(b *testing.B)    { benchmarkClientProcessResponse(1, false, b) }
func BenchmarkClientProcessResponse10(b *testing.B)   { benchmarkClientProcessResponse(10, false, b) }
func BenchmarkClientProcessResponse100(b *testing.B)  { benchmarkClientProcessResponse(100, false, b) }
func BenchmarkClientProcessResponse1000(b *testing.B) { benchmarkClientProcessResponse(1000, false, b) }
func BenchmarkClientProcessResponse10000(b *testing.B) {
	benchmarkClientProcessResponse(10000, false, b)
}
func BenchmarkClientProcessResponseIntersection1(b *testing.B) {
	benchmarkClientProcessResponse(1, true, b)
}
func BenchmarkClientProcessResponseIntersection10(b *testing.B) {
	benchmarkClientProcessResponse(10, true, b)
}
func BenchmarkClientProcessResponseIntersection100(b *testing.B) {
	benchmarkClientProcessResponse(100, true, b)
}
func BenchmarkClientProcessResponseIntersection1000(b *testing.B) {
	benchmarkClientProcessResponse(1000, true, b)
}
func BenchmarkClientProcessResponseIntersection10000(b *testing.B) {
	benchmarkClientProcessResponse(10000, true, b)
}
