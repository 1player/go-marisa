package marisa

import (
	"testing"
)

func TestTrie(t *testing.T) {
	keyset := NewKeyset()
	keyset.Push_back("hello", int64(5))

	trie := NewTrie()
	trie.Build(keyset)
}
