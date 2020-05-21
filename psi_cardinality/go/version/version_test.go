package version

import (
	"regexp"
	"testing"
)

func TestVersion(t *testing.T) {
	matched, _ := regexp.MatchString(`[0-9]+[.][0-9]+[.][0-9]+(-[A-Za-z0-9]+)?`, Version())
	if !matched {
		t.Errorf("Got invalid version %v", Version())
	}
}
