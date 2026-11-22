Implement a consistent hash ring:

```cpp
class HashRing {
public:
    explicit HashRing(int virtual_nodes = 50);

    void add_node(const std::string& name);
    void remove_node(const std::string& name);

    std::string locate(const std::string& key) const;  // "" when the ring is empty
    std::size_t size() const;                          // number of distinct nodes
};
```

Rules:

- Ring positions are in `[0, 360)`.
- Each node gets `virtual_nodes` replicas; replica `i` has the string `name + "#" + std::to_string(i)` (starting at 0).
- Compute the 32-bit FNV-1a hash (initial value `2166136261u`; for each byte, XOR then multiply by `16777619u`), then mix it with 32-bit wraparound:
  `h ^= h >> 16; h *= 0x7feb352du; h ^= h >> 15; h *= 0x846ca68bu; h ^= h >> 16;`
  and take the result modulo 360.
- `locate(key)` hashes `key % 360` and finds the first replica position at least that large, wrapping to the smallest replica past the end. It returns the owning node name.
- Adding an existing node changes nothing; removing an unknown node is a no-op. Adding a node twice must not create duplicate replicas.
- `virtual_nodes < 1` is treated as 1.

The judge checks these invariants, including that adding or removing a node only remaps the keys that must move. **Do not write `main`.**

### Input

None.

### Output

None.

### Example

```cpp
HashRing ring(50);
ring.add_node("a");
ring.locate("user:1");   // "a"
ring.add_node("b");
ring.locate("user:1");   // may still be "a"
```

### Constraints

- Node names are lowercase letters, at most 20 characters.
- Tests use at most 8 nodes and at most 200 replicas.
