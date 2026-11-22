This exercise reads no stdin and writes no stdout. Implement a simple linear allocator (arena); a hidden harness runs the tests.

Implement:

```cpp
class Arena {
public:
    explicit Arena(int capacity);  // 1 <= capacity <= 256

    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;

    // Allocate size bytes aligned to alignment.
    // Return nullptr when size <= 0 or space is insufficient, without changing used().
    void* allocate(int size, int alignment);

    int used() const;      // bytes used, including alignment padding
    int capacity() const;  // capacity given at construction
    void reset();          // reclaim everything
};
```

Requirements:

- `alignment` is guaranteed to be a power of two no greater than 16 (such as 1, 2, 4, 8, 16);
- Use a real fixed byte buffer internally; do not call `new` or use `std::vector` for each allocation;
- Round the offset up to the alignment boundary before reserving `size` bytes;
- On failure, `used()` must not change;
- Every returned address must satisfy `address % alignment == 0`;
- Do not write `main`.

Hint: declare the internal buffer with `alignas(16)`. The padding formula is `(offset + alignment - 1) / alignment * alignment`, or bit arithmetic when `alignment` is a power of two.
