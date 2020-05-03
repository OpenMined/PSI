package bloomfilter

import (
	"testing"
)

func TestBloomFilterSanity(t *testing.T) {
	bf, err := Create(0.1, 100)
	if err != nil {
		t.Errorf("Failed to create a bloom filter %v", err)
	}
	if bf == nil {
		t.Errorf("Failed to create a bloom filter: nil")
	}
	defer bf.Destroy()

	if bf.NumHashFunctions() == 0 {
		t.Errorf("invalid bloom filter: nil")
	}
}

func TestBloomFilterFailure(t *testing.T) {
	bf := BloomFilter{}

	err := bf.Add("a")
	if err == nil {
		t.Errorf("adding values to an invalid context should fail")
	}

	err = bf.AddArray([]string{"a", "b"})
	if err == nil {
		t.Errorf("adding values to an invalid context should fail")
	}

	_, err = bf.Check("a")
	if err == nil {
		t.Errorf("checking values in an invalid context should fail")
	}

	_, err = bf.ToJSON()
	if err == nil {
		t.Errorf("create json should fail with an invalid context")
	}

	err = bf.Destroy()
	if err == nil {
		t.Errorf("destroy should fail with an invalid context")
	}

	cnt := bf.NumHashFunctions()
	if cnt != 0 {
		t.Errorf("NumHashFunctions should fail with an invalid context")
	}
	_, err = Create(3, 100)
	if err == nil {
		t.Errorf("Create should fail with an invalid fpr")
	}

	_, err = CreateFromJSON("abc")
	if err == nil {
		t.Errorf("CreateFromJSON should fail with an invalid input")
	}
}

func TestBloomFilterAdd(t *testing.T) {
	bf, err := Create(0.001, 1<<10)
	if err != nil || bf == nil {
		t.Errorf("Failed to create a bloom filter %v", err)
	}
	defer bf.Destroy()

	err = bf.Add("a")
	if err != nil {
		t.Errorf("Failed to add element to the bloom filter %v", err)
	}

	valid, err := bf.Check("a")
	if valid == false || err != nil {
		t.Errorf("Failed to add element to bloom filter")
	}
	invalid, err := bf.Check("b")
	if invalid == true || err != nil {
		t.Errorf("bloom filter check fail")
	}
}

func TestBloomFilterAddArray(t *testing.T) {
	bf, err := Create(0.001, 1<<10)
	if err != nil || bf == nil {
		t.Errorf("Failed to create a bloom filter %v", err)
	}
	defer bf.Destroy()

	err = bf.AddArray([]string{"a", "b", "c", "d"})
	if err != nil {
		t.Errorf("Failed to add array to the bloom filter %v", err)
	}

	for _, val := range []string{"a", "b", "b", "d"} {
		valid, err := bf.Check(val)
		if valid == false || err != nil {
			t.Errorf("Failed to add element to bloom filter")
		}
	}
	invalid, err := bf.Check("z")
	if invalid == true || err != nil {
		t.Errorf("bloom filter check fail")
	}
}

func TestBloomFilterFPR(t *testing.T) {
	for maxElements := 1 << 10; maxElements < (1 << 20); maxElements *= 2 {
		targetFPR := 0.1
		bf, err := Create(targetFPR, int64(maxElements))
		if err != nil || bf == nil {
			t.Errorf("Failed to create a bloom filter %v", err)
		}

		for i := 0; i < maxElements; i++ {
			bf.Add("Element " + string(i))
		}

		count := 0
		numTests := 10000
		for i := 0; i < numTests; i++ {
			found, _ := bf.Check("Test " + string(i))
			if found {
				count++
			}
		}

		actualFPR := float64(count / numTests)
		if actualFPR > (1.2 * targetFPR) {
			t.Errorf("FPR test fail. actual %v expected %v", actualFPR, 1.2*targetFPR)
		}
		bf.Destroy()
	}
}

func TestBloomFilterToJSON(t *testing.T) {
	bf, err := Create(0.01, 100)
	if err != nil || bf == nil {
		t.Errorf("Failed to create a bloom filter %v", err)
	}
	defer bf.Destroy()

	for i := 0; i < 100; i++ {
		bf.Add("Element " + string(i))
	}
	encoded, err := bf.ToJSON()
	if err != nil {
		t.Errorf("Failed to create json %v", err)
	}
	decoded, err := CreateFromJSON(encoded)
	if err != nil || decoded == nil {
		t.Errorf("Failed to decode bloom filter %v", err)
	}
	defer decoded.Destroy()

	for i := 0; i < 100; i++ {
		found, err := decoded.Check("Element " + string(i))
		if found == false || err != nil {
			t.Errorf("invalid BF decoded")
		}
	}
}
