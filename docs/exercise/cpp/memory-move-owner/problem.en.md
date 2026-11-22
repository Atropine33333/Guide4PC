This exercise reads no stdin and writes no stdout. Implement a dynamically allocated integer array that is movable but not copyable; a hidden harness runs the tests.

Implement:

```cpp
class UniqueIntArray {
public:
    explicit UniqueIntArray(int n);  // n >= 0, elements zero-initialized
    ~UniqueIntArray();

    UniqueIntArray(const UniqueIntArray&) = delete;
    UniqueIntArray& operator=(const UniqueIntArray&) = delete;

    UniqueIntArray(UniqueIntArray&& other) noexcept;
    UniqueIntArray& operator=(UniqueIntArray&& other) noexcept;

    int size() const;
    int* data();
    const int* data() const;
    void set(int index, int value);  // 0 <= index < size()
    int get(int index) const;        // 0 <= index < size()
};
```

After a move:

- The destination takes over the buffer, length, and all elements;
- The source's `size()` becomes 0 and `data()` becomes `nullptr`;
- No double free is allowed; a zero-length array also has `data() == nullptr`.

Do not write `main`.
