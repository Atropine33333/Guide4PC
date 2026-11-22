This exercise reads no stdin and writes no stdout. Implement a growing integer array that manages its own memory; a hidden harness runs the tests.

```cpp
class IntVector {
public:
    IntVector();
    ~IntVector();
    IntVector(const IntVector& other);
    IntVector& operator=(const IntVector& other);
    IntVector(IntVector&& other) noexcept;
    IntVector& operator=(IntVector&& other) noexcept;

    void push_back(int value);
    int size() const;
    int capacity() const;
    void clear();
    int& operator[](int index);
    const int& operator[](int index) const;
    int* data();
    const int* data() const;

private:
    int* data_;
    int size_;
    int capacity_;
};
```

Rules (rule of five):

- Default construction: `size() == 0`, `capacity() == 0`, `data() == nullptr`.
- `push_back` appends at the end; when capacity is exhausted, reallocate (capacity at least 1, and always `capacity() >= size()` afterwards). Existing elements keep their order.
- The destructor frees memory with `delete[]` and must be safe on an empty array.
- The copy constructor and copy assignment perform a deep copy: the two arrays are independent afterwards.
- The move constructor and move assignment are `noexcept`: steal the pointer and leave the source empty (`size() == 0`, `data() == nullptr`).
- Self-assignment is safe and keeps the values.
- `clear()` sets `size()` to 0 without necessarily releasing capacity; `push_back` must work normally afterwards.
- `operator[]` is only called with a valid index; `data()` returns the address of the contiguous storage.
- Do not write `main`.

### Hint

To grow, allocate a new array, copy the old elements one by one, then `delete[]` the old array. After a move, set all three members of the source to the empty state.
