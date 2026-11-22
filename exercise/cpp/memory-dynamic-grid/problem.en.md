This exercise reads no stdin. Implement an integer grid whose dimensions are decided at run time.

```cpp
#include <vector>

class Grid {
public:
    Grid(int rows, int cols);
    int rows() const;
    int cols() const;
    int& at(int row, int col);
    const int& at(int row, int col) const;
    void fill(int value);
    std::vector<int> row_major() const;
};
```

Rules:

- All elements live in **one contiguous** `std::vector<int>` in row-major order: index `row * cols + col`.
- `rows` / `cols` return the dimensions passed to the constructor.
- `at` returns a reference to the element; the harness only passes valid indices, so bounds checking is unnecessary.
- `fill` sets every element to `value`.
- `row_major` returns a copy of the elements in row-major order (length `rows * cols`).
- A copied grid must be independent of the original (deep copy).
- Do not write `main`.
