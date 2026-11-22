(Two comedians, one table, one fan.)

A: Tonight, a cache.

B: Go on.

A: Fixed capacity, keys and values. Full up, and a new key arrives — somebody gets thrown out.

B: Who?

A: Whoever has gone untouched longest. They call it LRU.

B: And a `get`?

A: Value comes back, and that key is marked "just used". No value, you say there is nothing and return `false`.

B: And `put`?

A: Key exists: change the value, mark it "just used". Key is new: put it in. Full? Throw somebody out first, then settle the newcomer.

B: What about capacity zero?

A: Ah, good question. Capacity zero: `put` does nothing, `get` always `false`, `size()` stays zero.

B: Sounds easy.

A: The trouble is those two words, "just used". A read counts, a write counts, an insert counts. Miss one and you evict the wrong fellow.

B: How does the judge check?

A: A fixed-seed random run, then a line-by-line comparison with a reference implementation.

**In plain words**: Complete `LruCache`: `get(key, out)` writes the value, marks the key most recently used, and returns `true` on a hit, `false` otherwise; `put` inserts or overwrites, evicting the least recently used key when full, and does nothing at capacity 0; `size()` and `capacity()` report honestly.

```cpp
class LruCache {
public:
    explicit LruCache(int capacity);

    bool get(int key, int& out);
    void put(int key, int value);

    int size() const;
    int capacity() const;
};
```

### Rules

- The capacity may be `0`.
- `get` writes the value to `out`, marks the key most recently used, and returns `true` on a hit; it returns `false` when the key is absent.
- `put` inserts or overwrites: an existing key is updated and marked most recently used; when the cache is full and a new key arrives, the least recently used key is evicted first. At capacity 0 it does nothing.
- `size` returns the number of keys; `capacity` returns the capacity passed to the constructor.
- A hidden harness interleaves `get` and `put`, checks that the evicted key is exactly the least recently used one, and compares against a reference implementation on a fixed-seed random sequence.
- Do not write `main`.
