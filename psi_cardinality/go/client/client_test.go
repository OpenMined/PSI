package client

import (
	"github.com/openmined/psi-cardinality/server"
	"testing"
)

func TestClientSanity(t *testing.T) {
	c, err := Create()
	if err != nil {
		t.Errorf("Failed to create a PSI client %v", err)
	}
	if c == nil {
		t.Errorf("Failed to create a PSI client: nil")
	}
	defer c.Destroy()
}

func TestClientFailure(t *testing.T) {
	c := &PSICardinalityClient{}
	_, err := c.CreateRequest([]string{"dummy"})
	if err == nil {
		t.Errorf("CreateRequest with an invalid context should fail")
	}
	_, err = c.ProcessResponse("dummy1", "dummy2")
	if err == nil {
		t.Errorf("ProcessResponse with an invalid context should fail")
	}
	err = c.Destroy()
	if err == nil {
		t.Errorf("Destroy with an invalid context should fail")
	}
	c, _ = Create()
	_, err = c.ProcessResponse("dummy1", "dummy2")
	if err == nil {
		t.Errorf("ProcessResponse with invalid input should fail")
	}
}

func TestClientServer(t *testing.T) {
	client, err := Create()
	if err != nil || client == nil {
		t.Errorf("Failed to create a PSI client %v", err)
	}
	defer client.Destroy()

	server, err := server.CreateWithNewKey()
	if err != nil || server == nil {
		t.Errorf("Failed to create a PSI server %v", err)
	}
	defer server.Destroy()

	generateItems := func(cnt int, m int) (int, []string) {
		items := []string{}
		for i := 0; i < cnt; i++ {
			items = append(items, "Element "+string(m*i))
		}
		return cnt, items
	}
	cntClientItems, clientItems := generateItems(1000, 1)
	_, serverItems := generateItems(10000, 2)

	setup, err := server.CreateSetupMessage(0.01, int64(cntClientItems), serverItems)
	if err != nil {
		t.Errorf("failed to create setup msg %v", err)
	}
	request, err := client.CreateRequest(clientItems)
	if err != nil {
		t.Errorf("failed to create request %v", err)
	}
	serverResp, err := server.ProcessRequest(request)
	if err != nil {
		t.Errorf("failed to process request %v", err)
	}
	intersectionCnt, err := client.ProcessResponse(setup, serverResp)
	if err != nil {
		t.Errorf("failed to compute intersection %v", err)
	}

	if int(intersectionCnt) < (cntClientItems / 2) {
		t.Errorf("Invalid intersection. expected lower bound %v. got %v", (cntClientItems / 2), intersectionCnt)
	}

	if float64(intersectionCnt) > float64(cntClientItems/2)*float64(1.1) {
		t.Errorf("Invalid intersection. expected upper bound %v. got %v", float64(cntClientItems/2)*float64(1.1), intersectionCnt)
	}
}

var result string

func benchmarkClientCreateRequest(cnt int, b *testing.B) {
	b.ReportAllocs()
	total := 0
	for n := 0; n < b.N; n++ {
		client, err := Create()
		if err != nil || client == nil {
			b.Errorf("failed to get client")
		}
		inputs := []string{}
		for i := 0; i < cnt; i++ {
			inputs = append(inputs, "Element "+string(i))
		}
		request, err := client.CreateRequest(inputs)
		if err != nil {
			b.Errorf("failed to generate request")
		}

		//ugly hack for preventing compiler optimizations
		result = request

		total += cnt
		b.ReportMetric(float64(len(request)), "RequestSize")

	}
	b.ReportMetric(float64(total), "ElementsProcessed")
}

func BenchmarkClientCreateRequest1(b *testing.B)     { benchmarkClientCreateRequest(1, b) }
func BenchmarkClientCreateRequest10(b *testing.B)    { benchmarkClientCreateRequest(10, b) }
func BenchmarkClientCreateRequest100(b *testing.B)   { benchmarkClientCreateRequest(100, b) }
func BenchmarkClientCreateRequest1000(b *testing.B)  { benchmarkClientCreateRequest(1000, b) }
func BenchmarkClientCreateRequest10000(b *testing.B) { benchmarkClientCreateRequest(10000, b) }

var dummyInt64 int64

func benchmarkClientProcessResponse(cnt int, b *testing.B) {
	b.ReportAllocs()
	total := 0
	for n := 0; n < b.N; n++ {
		client, err := Create()
		if err != nil || client == nil {
			b.Errorf("failed to get client")
		}
		server, err := server.CreateWithNewKey()
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
		intersectionCnt, err := client.ProcessResponse(setup, serverResp)
		if err != nil {
			b.Errorf("failed to process response %v", err)
		}
		total += cnt
		//ugly hack for preventing compiler optimizations
		dummyInt64 = intersectionCnt
	}
	b.ReportMetric(float64(total), "ElementsProcessed")
}

func BenchmarkClientProcessResponse1(b *testing.B)     { benchmarkClientProcessResponse(1, b) }
func BenchmarkClientProcessResponse10(b *testing.B)    { benchmarkClientProcessResponse(10, b) }
func BenchmarkClientProcessResponse100(b *testing.B)   { benchmarkClientProcessResponse(100, b) }
func BenchmarkClientProcessResponse1000(b *testing.B)  { benchmarkClientProcessResponse(1000, b) }
func BenchmarkClientProcessResponse10000(b *testing.B) { benchmarkClientProcessResponse(10000, b) }
