package server

import (
	"bytes"
	"github.com/openmined/psi/client"
	psi_ds "github.com/openmined/psi/datastructure"
	psi_proto "github.com/openmined/psi/pb"
	"regexp"
	"testing"
)

func generateItems(cnt int, m int) (int, []string) {
	items := []string{}
	for i := 0; i < cnt; i++ {
		items = append(items, "Element "+string(m*i))
	}
	return cnt, items
}

func generateSet(in []int64) map[int]struct{} {
	out := map[int]struct{}{}
	for _, item := range in {
		out[int(item)] = struct{}{}
	}

	return out
}
func testServerSanity(t *testing.T, revealIntersection bool) {
	server, err := CreateWithNewKey(revealIntersection)
	if err != nil || server == nil {
		t.Errorf("Failed to create a PSI server %v", err)
	}
	_, err = server.CreateSetupMessage(0.01, 10, []string{}, psi_ds.Gcs)
	if err != nil {
		t.Errorf("Should not fail on empty input %v", err)
	}
	key, err := server.GetPrivateKeyBytes()
	if err != nil {
		t.Errorf("Failed to create a PSI server key %v", err)
	}

	newServer, err := CreateFromKey(key, revealIntersection)
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

	matched, _ := regexp.MatchString(`[0-9]+[.][0-9]+[.][0-9]+(-[A-Za-z0-9]+)?`, server.Version())
	if !matched {
		t.Errorf("Got invalid version %v", server.Version())
	}
}
func TestServerSanity(t *testing.T) {
	testServerSanity(t, false)
	testServerSanity(t, true)
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

func testServerClient(t *testing.T, revealIntersection bool) {
	client, err := client.CreateWithNewKey(revealIntersection)
	if err != nil || client == nil {
		t.Errorf("Failed to create a PSI client %v", err)
	}

	server, err := CreateWithNewKey(revealIntersection)
	if err != nil || server == nil {
		t.Errorf("Failed to create a PSI server %v", err)
	}

	cntClientItems, clientItems := generateItems(10, 1)
	_, serverItems := generateItems(100, 2)

	fpr := 1. / (1000000000)
	setup, err := server.CreateSetupMessage(fpr, int64(cntClientItems), serverItems, psi_ds.Gcs)
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

	if revealIntersection {
		intersection, err := client.GetIntersection(setup, serverResp)
		if err != nil {
			t.Errorf("failed to compute intersection %v", err)
		}
		intersectionSet := generateSet(intersection)
		for idx := 0; idx < cntClientItems; idx++ {
			_, ok := intersectionSet[idx]
			if ok != (idx%2 == 0) {
				t.Errorf("Invalid intersection for item %v", idx)
			}
		}
	} else {
		intersectionCnt, err := client.GetIntersectionSize(setup, serverResp)
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
}

func TestServerClient(t *testing.T) {
	testServerClient(t, false)
	testServerClient(t, true)
}
