This exercise reads no stdin and writes no stdout. Implement a matrix class whose row and column counts are template parameters, tested by a hidden harness.

```cpp
template <typename T, int Rows, int Cols>
class Matrix {
public:
    Matrix();
    T& at(int row, int col);
    const T& at(int row, int col) const;
    int rows() const;
    int cols() const;
    T sum() const;
    void fill(const T& value);
};
```

Rules:

- Elements are stored row by row; `Rows` and `Cols` are both positive.
- A default-constructed matrix holds `T{}` in every element (0 for `int`, 0.0 for `double`).
- `at(row, col)` returns a reference that can be assigned through; tests only pass valid indices.
- `rows()` and `cols()` return the template parameters.
- `sum()` returns the sum of all elements, with the element type.
- `fill(value)` sets every element to `value`.
- Do not write `main`.

### Example

| Call | Result |
| --- | --- |
| `Matrix<int, 2, 3>().rows()` / `.cols()` | `2` / `3` |
| `Matrix<int, 2, 2> m; m.fill(7); m.sum()` | `28` |
| `Matrix<double, 1, 1> d; d.at(0, 0) = 2.5; d.sum()` | `2.5` |

### Hint

Store `T data_[Rows * Cols]{};` privately and return `data_[row * Cols + col]` from `at`. Provide both a `const` and a non-`const` overload of `at` so a `const Matrix&` can still read elements.
