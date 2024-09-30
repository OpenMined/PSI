package server

import (
	"testing"

	"github.com/openmined/psi/client"
	psi_ds "github.com/openmined/psi/datastructure"
	psi_proto "github.com/openmined/psi/pb"
)

func generateItems(cnt int, m int) (int, []string) {
	items := []string{}
	for i := 0; i < cnt; i++ {
		items = append(items, "Element "+string(m*i))
	}
	return cnt, items
}

func testServerFailure(t *testing.T, revealIntersection bool) {
	server := &PsiServer{}
	_, err := server.GetPrivateKeyBytes()
	if err == nil {
		t.Errorf("GetPrivateKeyBytes should fail with an invalid context %v", err)
	}
	_, err = server.CreateSetupMessage(0.1, 100, []string{"dummy"}, psi_ds.Gcs)
	if err == nil {
		t.Errorf("CreateSetupMessage should fail with an invalid context %v", err)
	}

	_, err = server.ProcessRequest(&psi_proto.Request{})
	if err == nil {
		t.Errorf("ProcessRequest should fail with an invalid context %v", err)
	}

	client, err := client.CreateWithNewKey(!revealIntersection)
	if err != nil || client == nil {
		t.Errorf("Failed to create a PSI client %v", err)
	}

	_, clientItems := generateItems(1000, 1)

	request, err := client.CreateRequest(clientItems)
	if err != nil {
		t.Errorf("failed to create request %v", err)
	}
	_, err = server.ProcessRequest(request)
	if err == nil {
		t.Errorf("should fail on invalid request %v", err)
	}
}
func TestServerFailure(t *testing.T) {
	testServerFailure(t, false)
	testServerFailure(t, true)
}
