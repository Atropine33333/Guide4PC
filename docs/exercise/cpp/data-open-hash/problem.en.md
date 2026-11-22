RULES FOR THE OPEN-ADDRESSING TABLE

Read every clause below before you begin filling this table. The rules of this table are all true; only common sense does not quite apply.

1. Do not write `main`.
2. There are no linked lists here. One slot holds one key-value pair; both key and value are `int`, and keys may be negative. A negative number enters, and a seat is found for it all the same.
3. Calling `put` on the same key again changes only its value; the element count does not move.
4. When `get` finds the key it writes the value into `out` and returns `true`. When it does not, it returns `false` and leaves `out` untouched — not one byte.
5. `erase` removes the key and returns `true`; if the key was never there, it returns `false`, and nothing else happens at all.
6. `size()` counts the key-value pairs currently present; be one out, and it shows.
7. When a key is removed, its slot keeps a "deleted" mark. Probing that reaches such a slot must not stop; keep going. Only a slot never occupied proves the key absent. Whoever stops there will lose a key one day.
8. When the table nears full, take a larger one and seat every still-occupied slot again. Slots bearing the deleted mark need not be invited.

**In plain words**: implement an integer hash map `OpenHashMap` with open addressing (linear probing). `put` writes or updates a key-value pair, and putting the same key again does not increase the count; `get` writes the value to `out` and returns `true` on a hit, and returns `false` without modifying `out` on a miss; `erase` returns `true` when it removes a key and `false` when the key is absent; `size` returns the number of pairs. A slot has three states — empty, occupied, deleted (tombstone). Probing must continue past a tombstone and stop only at a truly empty slot; when the load grows, rehash into a larger table, reinserting every slot still occupied. **Do not write `main`**.

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

### Rules

- Keys and values are `int`, and keys may be negative.
- Putting the same key again updates its value and does not increase the element count.
- `get` writes the value into `out` and returns `true` on a hit; on a miss it returns `false` and **does not modify** `out`.
- `erase` removes the key and returns `true`; it returns `false` when the key is absent.
- `size` returns the current number of key-value pairs.
- Open addressing (linear probing) is required; a gap left by a removal must not end probing early.
- Do not write `main`.

### Hint

A slot may record three states: empty, occupied, and deleted (a tombstone). Probing walks past a tombstone and stops only at a truly empty slot. When elements grow, rehash into a larger table and reinsert every slot still occupied.
