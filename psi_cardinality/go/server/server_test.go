package server

import (
	"github.com/openmined/psi-cardinality/client"
	"testing"
	"bytes"
)

func TestServerSanity(t *testing.T) {
	server, err := CreateWithNewKey()
	if err != nil || server == nil {
		t.Errorf("Failed to create a PSI server %v", err)
	}

	key, err := server.GetPrivateKeyBytes()
	if err != nil {
		t.Errorf("Failed to create a PSI server key %v", err)
	}

	newServer, err := CreateFromKey(key)
	if err != nil || newServer == nil {
		t.Errorf("Failed to create a PSI server from key %v", err)
	}

	newKey, err := newServer.GetPrivateKeyBytes()
	if err != nil {
		t.Errorf("Failed to create a new PSI server key %v", err)
	}
	if !bytes.Equal(key, newKey) {
		t.Errorf("new server invalid")
	}
	server.Destroy()
	for i := 0; i < 5; i++ {
		server.Destroy()
	}
}

func TestServerFailure(t *testing.T) {
	server := &PSICardinalityServer{}
	_, err := server.GetPrivateKeyBytes()
	if err == nil {
		t.Errorf("GetPrivateKeyBytes should fail with an invalid context %v", err)
	}
	_, err = server.CreateSetupMessage(0.1, 100, []string{"dummy"})
	if err == nil {
		t.Errorf("CreateSetupMessage should fail with an invalid context %v", err)
	}

	_, err = server.ProcessRequest("dummy")
	if err == nil {
		t.Errorf("ProcessRequest should fail with an invalid context %v", err)
	}

	server, _ = CreateWithNewKey()
	_, err = server.ProcessRequest("dummy")
	if err == nil {
		t.Errorf("ProcessRequest should fail with an invalid input %v", err)
	}
}

func TestServerClient(t *testing.T) {
	client, err := client.Create()
	if err != nil || client == nil {
		t.Errorf("Failed to create a PSI client %v", err)
	}

	server, err := CreateWithNewKey()
	if err != nil || server == nil {
		t.Errorf("Failed to create a PSI server %v", err)
	}

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

var dummyString string

func benchmarkServerSetup(cnt int, fpr float64, b *testing.B) {
	b.ReportAllocs()
	total := 0
	for n := 0; n < b.N; n++ {
		server, err := CreateWithNewKey()
		if err != nil || server == nil {
			b.Errorf("failed to get server")
		}

		inputs := []string{}
		for i := 0; i < cnt; i++ {
			inputs = append(inputs, "Element "+string(i))
		}

		numClientInputs := 10000
		setup, err := server.CreateSetupMessage(fpr, int64(numClientInputs), inputs)
		if err != nil {
			b.Errorf("failed to create setup msg %v", err)
		}
		total += cnt
		//ugly hack for preventing compiler optimizations
		dummyString = setup
	}
	b.ReportMetric(float64(total), "ElementsProcessed")
}

const fpr3 = 0.001
const fpr6 = 0.000001

func BenchmarkServerSetup1fpr3(b *testing.B)     { benchmarkServerSetup(10, fpr3, b) }
func BenchmarkServerSetup10fpr3(b *testing.B)    { benchmarkServerSetup(10, fpr3, b) }
func BenchmarkServerSetup100fpr3(b *testing.B)   { benchmarkServerSetup(100, fpr3, b) }
func BenchmarkServerSetup1000fpr3(b *testing.B)  { benchmarkServerSetup(1000, fpr3, b) }
func BenchmarkServerSetup10000fpr3(b *testing.B) { benchmarkServerSetup(10000, fpr3, b) }

func BenchmarkServerSetup1fpr6(b *testing.B)     { benchmarkServerSetup(10, fpr6, b) }
func BenchmarkServerSetup10fpr6(b *testing.B)    { benchmarkServerSetup(10, fpr6, b) }
func BenchmarkServerSetup100fpr6(b *testing.B)   { benchmarkServerSetup(100, fpr6, b) }
func BenchmarkServerSetup1000fpr6(b *testing.B)  { benchmarkServerSetup(1000, fpr6, b) }
func BenchmarkServerSetup10000fpr6(b *testing.B) { benchmarkServerSetup(10000, fpr6, b) }

func benchmarkServerProcessRequest(cnt int, b *testing.B) {
	b.ReportAllocs()
	total := 0
	for n := 0; n < b.N; n++ {
		client, err := client.Create()
		if err != nil || client == nil {
			b.Errorf("failed to get client")
		}
		server, err := CreateWithNewKey()
		if err != nil || server == nil {
			b.Errorf("failed to get server")
		}

		inputs := []string{}
		for i := 0; i < cnt; i++ {
			inputs = append(inputs, "Element "+string(i))
		}

		request, err := client.CreateRequest(inputs)
		if err != nil {
			b.Errorf("failed to create request %v", err)
		}
		serverResp, err := server.ProcessRequest(request)
		if err != nil {
			b.Errorf("failed to process request %v", err)
		}
		total += cnt
		b.ReportMetric(float64(len(serverResp)), "ResponseSize")
		//ugly hack for preventing compiler optimizations
		dummyString = serverResp
	}
	b.ReportMetric(float64(total), "ElementsProcessed")
}

func BenchmarkServerProcessRequest1(b *testing.B)     { benchmarkServerProcessRequest(1, b) }
func BenchmarkServerProcessRequest10(b *testing.B)    { benchmarkServerProcessRequest(10, b) }
func BenchmarkServerProcessRequest100(b *testing.B)   { benchmarkServerProcessRequest(100, b) }
func BenchmarkServerProcessRequest1000(b *testing.B)  { benchmarkServerProcessRequest(1000, b) }
func BenchmarkServerProcessRequest10000(b *testing.B) { benchmarkServerProcessRequest(10000, b) }
