# Bitmap User Manual

A single page is pinned to the wall of the machine room, its edge curling. The text is set out in numbered sections.

**1. Scope** This sheet records numbered bits, numbered from `0`, at most 256 of them. Each bit is either set or clear.

**2. Initialisation** `bitset_init` records the bit count `nbits` and clears every bit.

**3. Setting** `bitset_set` sets bit `i` and returns `0`. An out-of-range index returns `-1` and leaves the bitmap unchanged. Setting an already set bit still returns `0`.

**4. Clearing** `bitset_clear` clears bit `i` and returns `0`; out of range it returns `-1`. Clearing an already clear bit still returns `0`.

**5. Testing** `bitset_test` returns `1` for a set bit, `0` for a clear bit and `-1` out of range.

**6. Counting** `bitset_count` returns how many bits in `[0, nbits)` are set; padding bits outside that range are not counted.

**7. Notes** No memory is allocated; each byte holds 8 bits with the lower-numbered bit in the lower position; neighbouring bits are independent.

**In plain words**: implement a bitmap of up to 256 bits with initialisation, set, clear, test and count; out-of-range set and clear return `-1`, and so does an out-of-range test.

**Interface**:

```c
#define BITSET_MAX_BITS 256

typedef struct {
    unsigned char words[BITSET_MAX_BITS / 8];
    int nbits;
} BitSet;

void bitset_init(BitSet *b, int nbits);
int bitset_set(BitSet *b, int i);
int bitset_clear(BitSet *b, int i);
int bitset_test(const BitSet *b, int i);
int bitset_count(const BitSet *b);
```

### Example

After `bitset_init(&b, 13)`, set `7`, `8` and `12`; `bitset_count(&b)` is `3`. Clear `8` and the count is `2`, while `bitset_test(&b, 8)` is `0`.

### Contract

- Implement only these functions; do not write `main`.
- No dynamic allocation; the capacity is `BITSET_MAX_BITS`.
- Bit indices must lie in `[0, nbits)`; `nbits` is at most `BITSET_MAX_BITS`.
- An out-of-range call must not change the bitmap.
