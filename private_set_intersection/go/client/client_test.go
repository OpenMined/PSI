package client

import (
	"testing"

	psi_proto "github.com/openmined/psi/pb"
)

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
