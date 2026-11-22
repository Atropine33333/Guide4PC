This exercise reads no stdin and writes no stdout. Implement a fixed-size array class template; a hidden harness runs the tests.

Implement:

```cpp
template <typename T, int N>
class FixedArray {
public:
    FixedArray();       // value-initialize all N elements to T{}

    int size() const;   // returns N

    T& operator[](int index);
    const T& operator[](int index) const;

    void fill(const T& value);  // set every element to value

    // Return false without touching out when out of range; otherwise store and return true.
    bool try_get(int index, T& out) const;

    T* data();
    const T* data() const;
};
```

Requirements:

- Use real fixed storage (such as `T items[N]`), not a dynamically allocated `std::vector`;
- Default construction of `FixedArray<T, N>` must initialize `int` elements to 0 and `double` elements to 0.0;
- `operator[]` assumes a valid index; `try_get` must check `0 <= index < N`;
- Do not write `main`.

Hint: `T items_[N]{};` performs value initialization.
