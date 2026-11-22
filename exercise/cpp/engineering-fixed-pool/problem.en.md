FIXED ASSET MANAGEMENT OFFICE

Document No. 11 of 2026

NOTICE ON THE ISSUANCE OF THE MEASURES FOR THE MANAGEMENT OF THE FIXED INTEGER POOL

To all lending units and return windows:

For the purpose of standardising the lending and return of slots, and of putting an end to wild pointers, double returns, and inflated capacity reports, the Measures for the Management of the Fixed Integer Pool are hereby issued for your compliance.

Article 1. The pool fixes `capacity` slots once, at construction (`capacity >= 0`), and never grows or shrinks. When capacity is zero, every acquisition fails and every return is refused.

Article 2. `acquire(value)` scans from index 0 for the first free slot, marks it used, stores `value`, and returns its address; when the pool is full it returns a null pointer.

Article 3. `release(slot)` accepts only a slot currently lent out by this pool, and only on exact pointer equality. Foreign pointers, null pointers, and slots already returned are refused, and the books do not move.

Article 4. `live()` shall report the number on loan truthfully, and `capacity()` the total capacity truthfully. False reporting will be handled as fraud.

Article 5. The destructor frees the internal arrays and must be safe when capacity is zero. The copy constructor and copy assignment are deleted and shall not be supplied.

Article 6. Do not write `main`, and do not change the public interface.

**In plain words**: Complete `IntPool`. `slots_` stores the values and `used_` records whether each slot is checked out; `release` scans the slots and compares addresses for equality, rather than ordering unrelated pointers.

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

### Rules

- `capacity >= 0`; with capacity 0, `acquire` always returns `nullptr` and `release(nullptr)` returns `false`.
- The constructor allocates `capacity` slots, all initially unused.
- `acquire(value)` finds the first free slot, marks it used, stores `value`, and returns its address; when the pool is full it returns `nullptr`. Search for a free slot starting at index 0.
- `release(slot)` returns `true` only when `slot` points exactly at one of the pool's currently used slots, freeing it; for anything else (a foreign pointer, `nullptr`, an already released slot) it returns `false` and changes nothing.
- `live()` returns the number of currently acquired slots; `capacity()` returns the total capacity.
- The destructor frees the internal arrays and must be safe when capacity is 0.
- The copy constructor and copy assignment are deleted; do not implement them.
- Do not write `main`.

### Hint

`slots_` stores values and `used_` records whether each slot is checked out. `release` can scan the slots and compare addresses for equality.
