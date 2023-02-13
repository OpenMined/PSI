package client

import (
	"bytes"
	"regexp"
	"testing"

	psi_proto "github.com/openmined/psi/pb"
)

func testClientSanity(t *testing.T, revealIntersection bool) {
	client, err := CreateWithNewKey(revealIntersection)
	if err != nil || client == nil {
		t.Errorf("Failed to create a PSI client %v", err)
	}

	key, err := client.GetPrivateKeyBytes()
	if err != nil {
		t.Errorf("Failed to create a PSI client key %v", err)
	}

	newClient, err := CreateFromKey(key, revealIntersection)
	if err != nil || newClient == nil {
		t.Errorf("Failed to create a PSI client from key %v", err)
	}

	newKey, err := newClient.GetPrivateKeyBytes()
	if err != nil {
		t.Errorf("Failed to create a new PSI client key %v", err)
	}
	if !bytes.Equal(key, newKey) {
		t.Errorf("new client invalid")
	}
	client.Destroy()
	for i := 0; i < 5; i++ {
		client.Destroy()
	}

	matched, _ := regexp.MatchString(`[0-9]+[.][0-9]+[.][0-9]+(-[A-Za-z0-9]+)?`, client.Version())
	if !matched {
		t.Errorf("Got invalid version %v", client.Version())
	}
}

func TestClientSanity(t *testing.T) {
	testClientSanity(t, true)
	testClientSanity(t, false)
}

func testClientFailure(t *testing.T, revealIntersection bool) {
	c := &PsiClient{}
	_, err := c.CreateRequest([]string{"dummy"})
	if err == nil {
		t.Errorf("CreateRequest with an invalid context should fail")
	}
	dummy1 := &psi_proto.ServerSetup{}
	dummy2 := &psi_proto.Response{}
	if revealIntersection {
		_, err = c.GetIntersection(dummy1, dummy2)
		if err == nil {
			t.Errorf("GetIntersection with an invalid context should fail")
		}
	} else {
		_, err = c.GetIntersectionSize(dummy1, dummy2)
		if err == nil {
			t.Errorf("GetIntersectionSize with an invalid context should fail")
		}
	}
}

func TestClientFailure(t *testing.T) {
	testClientFailure(t, false)
	testClientFailure(t, true)
}
