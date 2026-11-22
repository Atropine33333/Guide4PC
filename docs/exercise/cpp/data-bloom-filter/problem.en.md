HOW TO USE THIS BLOOM FILTER

The instructions for this filter follow. Fill it in accordingly. Anything that contradicts them, the filter will not recognise.

Clause 1. This filter has `bits` bits, and `hash_count` is the number of hashes used per insertion or query. A constructor argument below 1 is treated as 1 — however small the number, a seat must be found.
Clause 2. `add` sets the `hash_count` hashed positions to 1. Insert the same value any number of times, and the same bits light up; no bit is ever cleared back to 0.
Clause 3. `maybe_contains` may say only two things: "possibly present" and "certainly absent". Any value that has been added must be answered "possibly present". Answer "certainly absent" and that is a false negative, which this filter will not tolerate.
Clause 4. False positives are allowed — a value never inserted may still be answered "possibly present". That is this filter's nature; it cannot be repaired, so do not try.
Clause 5. `set_bits()` reports how many bits are currently 1, and `bit_count()` reports the total. Both must agree with the bitmap.
Clause 6. This filter offers no removal, and the bitmap is not opened to the outside.
Clause 7. No stdin, no stdout, no change to the public interface, and no `main`. The instructions end here and will not be explained again.

**In plain words**: implement a fixed-size Bloom filter `BloomFilter`; it reads no stdin and writes no stdout, and a hidden harness runs the tests. `bits` is the bitmap length and `hash_count` is the number of hashes per element, both treated as 1 when below 1; `add` sets the positions given by `hash_count` deterministic hashes to 1, and inserting the same value again only lights the same bits without clearing any; `maybe_contains` may produce false positives but must never miss, so any inserted value returns `true`; `set_bits` returns how many bits are 1 and `bit_count` returns the total. Removal is not required, and the public interface must not change — **do not write `main`**.

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

### Rules

- The filter has `bits` bits. Constructor arguments below 1 are each treated as 1.
- `hash_count` is the number of probes per insertion or query; any deterministic string hash will do.
- `add` sets the corresponding bits to 1, and inserting the same value again clears no bit.
- `maybe_contains` may return only "possibly present" or "certainly absent"; a value already inserted must return `true` (no false negatives).
- `set_bits()` returns how many bits are 1, and `bit_count()` returns the total number of bits.
- Removal is not required, and the bitmap need not be accessible from outside.
- Do not write `main`, and do not modify the public interface.
