This exercise reads no stdin. Implement an integer hash map with open addressing (linear probing).

```cpp
#include <vector>

class OpenHashMap {
public:
    void put(int key, int value);
    bool get(int key, int& out) const;
    bool erase(int key);
    int size() const;
};
```

Rules:

- Keys and values are `int`; keys may be negative.
- Calling `put` again with the same key updates its value and does not change the element count.
- `get` writes the value to `out` and returns `true` when the key exists; it returns `false` otherwise and **must not modify** `out`.
- `erase` removes the key and returns `true`; it returns `false` when the key is absent.
- `size` returns the number of stored key-value pairs.
- Open addressing (linear probing) is required; a slot freed by `erase` must not stop later probes early.
- Do not write `main`.

### Hint

Give each slot three states: empty, occupied, and deleted (tombstone). Probing passes tombstones and stops only at a genuinely empty slot. Grow and reinsert all occupied slots when the table gets full.
