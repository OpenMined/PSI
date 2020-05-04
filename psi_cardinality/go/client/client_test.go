package client

import (
	"openmined/psi-cardinality/server"
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
