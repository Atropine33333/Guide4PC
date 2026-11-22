This exercise reads no stdin and writes no stdout. Implement a fixed-capacity LRU (least recently used) cache; a hidden harness runs the tests.

Implement `LruCache`:

- `explicit LruCache(int capacity)`: capacity may be `0`.
- `bool get(int key, int& out)`: if the key exists, write its value to `out`, mark it as most recently used, and return `true`; otherwise return `false`.
- `void put(int key, int value)`: insert or overwrite. If the key exists, update the value and mark it most recently used; if the cache is full and a new key must be inserted, evict the least recently used key first. Do nothing when capacity is `0`.
- `int size() const`: return the number of keys.
- `int capacity() const`: return the capacity passed to the constructor.

The harness interleaves `get` and `put`, checks that the evicted key is exactly the least recently used one, and compares against a reference implementation on a fixed-seed random sequence.
