This exercise reads no stdin and writes no stdout. Implement a fixed-capacity pool of integer slots; a hidden harness runs the tests.

```cpp
class IntPool {
public:
    explicit IntPool(int capacity);
    ~IntPool();

    IntPool(const IntPool&) = delete;
    IntPool& operator=(const IntPool&) = delete;

    int* acquire(int value);
    bool release(int* slot);
    int live() const;
    int capacity() const;

private:
    int* slots_;
    bool* used_;
    int capacity_;
    int live_;
};
```

Rules:

- `capacity >= 0`; with capacity 0, `acquire` always returns `nullptr` and `release(nullptr)` returns `false`.
- The constructor allocates `capacity` slots, all initially unused.
- `acquire(value)` finds the first free slot, marks it used, stores `value`, and returns its address; when the pool is full it returns `nullptr`. Search for a free slot starting at index 0.
- `release(slot)` returns `true` only when `slot` points exactly at one of the pool's **currently used** slots, freeing it; for anything else (a foreign pointer, `nullptr`, an already released slot) it returns `false` and changes nothing.
- `live()` returns the number of currently acquired slots; `capacity()` returns the total capacity.
- The destructor frees the internal arrays and must be safe when capacity is 0.
- The copy constructor and copy assignment are deleted; do not implement them.
- Do not write `main`.

### Hint

`slots_` stores values and `used_` records whether each slot is checked out. `release` can scan the slots and compare addresses for equality instead of ordering unrelated pointers.
