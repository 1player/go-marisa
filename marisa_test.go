package marisa

import "testing"

func TestCommonPrefixSearch(t *testing.T) {
	keyset := NewKeyset()
	keyset.PushBackStringWithWeight("a", 2)
	keyset.PushBackString("app")
	keyset.PushBackString("apple")

	trie := NewTrie()
	trie.Build(keyset)

	agent := NewAgent()
	agent.SetQueryString("apple")

	tests := []struct {
		prefix string
		id     int64
	}{
		{"a", 0},
		{"app", 1},
		{"apple", 2},
	}
	i := 0

	for i = 0; i < len(tests) && trie.CommonPrefixSearch(agent); i++ {
		key := agent.Key()
		test := tests[i]

		if prefix := key.Str(); prefix != test.prefix {
			t.Errorf("#%d, expected prefix %s, got %s\n", i, test.prefix, prefix)
		}
		if id := key.Id(); id != test.id {
			t.Errorf("#%d, expected id %d, got %d\n", i, test.id, id)
		}
	}

	if i != len(tests) {
		t.Errorf("Got %d prefixes, expected %d\n", i, len(tests))
	}
}
