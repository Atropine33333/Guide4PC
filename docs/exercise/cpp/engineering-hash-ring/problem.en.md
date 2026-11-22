(The diviner wipes the round table, all three hundred and sixty degrees of it, and begins to shuffle.)

Querent: what is this reading for?

Diviner: for a name, and which of the watching gods it falls under. The table is marked with 360 degrees, counted from 0 to 359.

Querent: how are the gods placed?

Diviner: each god holds `virtual_nodes` places, its copies numbered from 0 and written as `name#i`. Give the name to FNV-1a: start at `2166136261u`, XOR each byte, then multiply by `16777619u`, wrapping at 32 bits. Then mix the face three times: `h ^= h >> 16`, multiply by `0x7feb352du`; `h ^= h >> 15`, multiply by `0x846ca68bu`; `h ^= h >> 16`. Take the remainder modulo 360.

Querent: and the name I ask about?

Diviner: it makes the same journey. Where it lands, look clockwise for the first copy at or after that degree; if the end is passed, wrap to the smallest copy. Whoever holds the face owns the name.

Querent: and adding or removing a god?

Diviner: adding a god already present leaves the table still; removing one who is absent leaves it still. Adding twice creates no duplicate copy. A `virtual_nodes` below 1 counts as 1.

Querent: will the faces shift?

Diviner: after a change, only the names that must change hands do so. That is the craft.

**In plain words**: Complete `HashRing`: the FNV-1a hash and 32-bit mix, `add_node`/`remove_node`, and `locate` with its clockwise search and wrap-around; return `""` when the ring is empty. Do not write `main`.

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

### Rules

- Ring positions are in `[0, 360)`.
- Each node gets `virtual_nodes` replicas; replica `i` has the string `name + "#" + std::to_string(i)` (starting at 0).
- Compute the 32-bit FNV-1a hash (initial value `2166136261u`; for each byte, XOR then multiply by `16777619u`), then mix it with 32-bit wraparound: `h ^= h >> 16; h *= 0x7feb352du; h ^= h >> 15; h *= 0x846ca68bu; h ^= h >> 16;` and take the result modulo 360.
- `locate(key)` hashes `key % 360` and finds the first replica position at least that large, wrapping to the smallest replica past the end. It returns the owning node name.
- Adding an existing node changes nothing; removing an unknown node is a no-op. Adding a node twice must not create duplicate replicas.
- `virtual_nodes < 1` is treated as 1.
- These invariants must hold, including that adding or removing a node only remaps the keys that must move. Do not write `main`.

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
- At most 8 nodes and at most 200 replicas.
