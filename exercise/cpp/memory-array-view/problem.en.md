This exercise reads no stdin and writes no stdout. Implement a non-owning array view; a hidden harness runs the tests.

```cpp
class IntView {
public:
    IntView();
    IntView(int* data, int count);

    int* data() const;
    int size() const;
    bool empty() const;

    int& operator[](int index);
    const int& operator[](int index) const;

    IntView slice(int begin, int count) const;
};
```

Rules:

- A default-constructed view is empty: `data()` is `nullptr` and `size()` is 0.
- The constructor also produces an empty view when `data == nullptr` or `count <= 0`.
- A view never copies data; it stores only a pointer and a length. Mutating an element through the view mutates the original array.
- `slice(begin, count)` returns a view of `count` elements starting at `begin`, sharing memory with the original view; `slice` must not modify the original view.
- Return an empty view for an out-of-range slice (`begin < 0`, `count < 0`, `begin >= size()`, `begin + count > size()`) or `count == 0`.
- `operator[]` is only called with a valid index; no bounds checking is required.
- Do not write `main`.

### Hint

The view returned by `slice` only needs `data_ + begin` and `count`. Be careful not to overflow the type used for the `begin + count` check.
