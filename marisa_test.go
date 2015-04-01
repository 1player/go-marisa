package marisa

import (
	"sort"
	"testing"
)

func compareStringSlices(a, b []string) bool {
	if len(a) != len(b) {
		return false
	}

	sort.Strings(a)
	sort.Strings(b)

	for i := 0; i < len(a); i++ {
		if a[i] != b[i] {
			return false
		}
	}

	return true
}

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

func TestPredictiveSearch(t *testing.T) {
	keyset := NewKeyset()
	keyset.PushBackString("foo")
	keyset.PushBackString("foobar")
	keyset.PushBackString("foobaz")
	keyset.PushBackString("abcdef")

	trie := NewTrie()
	trie.Build(keyset)

	tests := []struct {
		query   string
		results []string
	}{
		{"fo", []string{"foo", "foobar", "foobaz"}},
		{"abcd", []string{"abcdef"}},
		{"123", []string{}},
		{"", []string{"abcdef", "foo", "foobar", "foobaz"}},
	}

	for _, test := range tests {
		agent := NewAgent()
		agent.SetQueryString(test.query)

		var found []string
		for trie.PredictiveSearch(agent) {
			found = append(found, agent.Key().Str())
		}

		if !compareStringSlices(found, test.results) {
			t.Errorf("Trie.PredictiveSearch(%q), expected %v, got %v\n", test.query, test.results, found)
		}
	}
}
