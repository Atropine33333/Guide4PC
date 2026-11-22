This task reads no stdin and writes no stdout. Implement a fixed-capacity slot store that detects dangling handles with generation counters.

```cpp
#include <cstddef>
#include <cstdint>

struct Handle {
    std::uint32_t index;
    std::uint32_t generation;
};

class HandleStore {
public:
    static constexpr std::uint32_t kCapacity = 8;
    static constexpr std::uint32_t kInvalidIndex = 0xffffffffu;

    HandleStore();

    Handle create(int value);
    bool valid(Handle handle) const;
    int* get(Handle handle);
    const int* get(Handle handle) const;
    bool destroy(Handle handle);
    std::size_t size() const;
    std::size_t capacity() const;
};
```

Rules:

- Slots are indexed `0` through `kCapacity - 1`, and each slot has a generation number starting at `1`.
- `create` finds a free slot, stores the value, and returns a handle with the slot's current generation. If no slot is free, return `{kInvalidIndex, 0}`.
- `destroy` releases a valid handle, increments that slot's generation, and returns `true`. It returns `false` for an invalid or stale handle and changes nothing.
- `valid` is `true` only when the index is in range, the generation matches, and the slot is occupied.
- `get` returns `nullptr` for an invalid handle and a pointer to the stored value for a valid handle.
- An old handle must stay invalid after its slot is reused, even when the new handle uses the same index.
- `size()` returns the number of occupied slots; `capacity()` returns `kCapacity`.
- Do not write `main`, and do not change the public interface.
