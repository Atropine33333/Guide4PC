Implement a lowercase-letter trie that counts prefixes.

```cpp
#include <string>

class Trie {
public:
    Trie();
    ~Trie();
    void insert(const std::string& word);
    int count_prefix(const std::string& prefix) const;
    int size() const;
    bool contains(const std::string& word) const;
};
```

Rules:

- Only strings over the lowercase letters `a`–`z` appear; strings may be empty.
- Every `insert` counts one word; inserting the same word twice counts twice.
- `count_prefix(prefix)` returns how many inserted words start with `prefix` (duplicates counted separately); an empty prefix returns `size()`.
- `contains(word)` reports whether the word was inserted at least once; the empty word is `true` only if it was inserted.
- `size()` returns the total number of insertions.
- Nodes must be released properly; do not rely on process exit to reclaim memory.
- Do not write `main`.

### Example

| Call sequence | Result |
| --- | --- |
| `insert("a"); insert("ab"); insert("abc")` | `count_prefix("a")` is 3, `count_prefix("ab")` is 2 |
| `insert("x"); insert("x")` | `size()` is 2, `count_prefix("x")` is 2 |
| `insert("cat"); insert("car")` | `contains("ca")` is `false`, `count_prefix("ca")` is 2 |

### Hint

A node holds 26 child pointers, a count of words passing through it, and a count of words ending there. The destructor recursively releases children; declare the copy constructor and copy assignment `= delete` so two objects cannot free the same nodes.
