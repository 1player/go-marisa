#ifndef MARISA_SWIG_H_
#define MARISA_SWIG_H_

#include <marisa.h>

namespace marisa_swig {

enum ErrorCode {
  OK = MARISA_OK,
  STATE_ERROR = MARISA_STATE_ERROR,
  NULL_ERROR = MARISA_NULL_ERROR,
  BOUND_ERROR = MARISA_BOUND_ERROR,
  RANGE_ERROR = MARISA_RANGE_ERROR,
  CODE_ERROR = MARISA_CODE_ERROR,
  RESET_ERROR = MARISA_RESET_ERROR,
  SIZE_ERROR = MARISA_SIZE_ERROR,
  MEMORY_ERROR = MARISA_MEMORY_ERROR,
  IO_ERROR = MARISA_IO_ERROR,
  FORMAT_ERROR = MARISA_FORMAT_ERROR
};

enum NumTries {
  MIN_NUM_TRIES = MARISA_MIN_NUM_TRIES,
  MAX_NUM_TRIES = MARISA_MAX_NUM_TRIES,
  DEFAULT_NUM_TRIES = MARISA_DEFAULT_NUM_TRIES
};

enum CacheLevel {
  HUGE_CACHE = MARISA_HUGE_CACHE,
  LARGE_CACHE = MARISA_LARGE_CACHE,
  NORMAL_CACHE = MARISA_NORMAL_CACHE,
  SMALL_CACHE = MARISA_SMALL_CACHE,
  TINY_CACHE = MARISA_TINY_CACHE,
  DEFAULT_CACHE = MARISA_DEFAULT_CACHE
};

enum TailMode {
  TEXT_TAIL = MARISA_TEXT_TAIL,
  BINARY_TAIL = MARISA_BINARY_TAIL,
  DEFAULT_TAIL = MARISA_DEFAULT_TAIL
};

enum NodeOrder {
  LABEL_ORDER = MARISA_LABEL_ORDER,
  WEIGHT_ORDER = MARISA_WEIGHT_ORDER,
  DEFAULT_ORDER = MARISA_DEFAULT_ORDER
};

class Key {
 public:
  void str(const char **ptr_out, std::size_t *length_out) const;
  std::size_t id() const;
  float weight() const;

 private:
  const marisa::Key key_;

  Key();
  Key(const Key &key);
  Key &operator=(const Key &);
};

class Query {
 public:
  void str(const char **ptr_out, std::size_t *length_out) const;
  std::size_t id() const;

 private:
  const marisa::Query query_;

  Query();
  Query(const Query &query);
  Query &operator=(const Query &);
};

class Keyset {
 friend class Trie;

 public:
  Keyset();
  ~Keyset();

  void push_back(const marisa::Key &key);
  void push_back(const char *ptr, std::size_t length, float weight = 1.0);

  const Key &key(std::size_t i) const;

  void key_str(std::size_t i,
      const char **ptr_out, std::size_t *length_out) const;
  std::size_t key_id(std::size_t i) const;

  std::size_t num_keys() const;

  bool empty() const;
  std::size_t size() const;
  std::size_t total_length() const;

  void reset();
  void clear();

 private:
  marisa::Keyset *keyset_;

  Keyset(const Keyset &);
  Keyset &operator=(const Keyset &);
};

class Agent {
 friend class Trie;

 public:
  Agent();
  ~Agent();

  void set_query(const char *ptr, std::size_t length);
  void set_query(std::size_t id);

  const Key &key() const;
  const Query &query() const;

  void key_str(const char **ptr_out, std::size_t *length_out) const;
  std::size_t key_id() const;

  void query_str(const char **ptr_out, std::size_t *length_out) const;
  std::size_t query_id() const;

 private:
  marisa::Agent *agent_;
  char *buf_;
  std::size_t buf_size_;

  Agent(const Agent &);
  Agent &operator=(const Agent &);
};

class Trie {
 public:
  Trie();
  ~Trie();

  void build(Keyset &keyset, int config_flags = 0);

  void mmap(const char *filename);
  void load(const char *filename);
  void save(const char *filename) const;

  bool lookup(Agent &agent) const;
  void reverse_lookup(Agent &agent) const;
  bool common_prefix_search(Agent &agent) const;
  bool predictive_search(Agent &agent) const;

  std::size_t lookup(const char *ptr, std::size_t length) const;
  void reverse_lookup(std::size_t id,
      const char **ptr_out_to_be_deleted, std::size_t *length_out) const;

  std::size_t num_tries() const;
  std::size_t num_keys() const;
  std::size_t num_nodes() const;

  TailMode tail_mode() const;
  NodeOrder node_order() const;

  bool empty() const;
  std::size_t size() const;
  std::size_t total_size() const;
  std::size_t io_size() const;

  void clear();

 private:
  marisa::Trie *trie_;

  Trie(const Trie &);
  Trie &operator=(const Trie &);
};

}  // namespace marisa_swig

#endif  // MARISA_SWIG_H_