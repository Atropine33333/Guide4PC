This task reads no stdin and writes no stdout. Implement a fixed-size Bloom filter.

```cpp
#include <cstddef>
#include <string>

class BloomFilter {
public:
    explicit BloomFilter(std::size_t bits, std::size_t hash_count);

    void add(const std::string& value);
    bool maybe_contains(const std::string& value) const;
    std::size_t set_bits() const;
    std::size_t bit_count() const;
};
```

Rules:

- The filter has `bits` bits. Constructor arguments below 1 are treated as 1.
- `hash_count` is the number of probes used by each insert/query; any deterministic string hash is acceptable.
- `add` sets the corresponding bits. Adding the same value again never clears a bit.
- `maybe_contains` may answer "possibly present" or "definitely absent"; it must return `true` for every value that was inserted (no false negatives).
- `set_bits()` returns the current number of set bits; `bit_count()` returns the total number of bits.
- Removal is not required, and the bitmap does not need to be exposed.
- Do not write `main`, and do not change the public interface.
