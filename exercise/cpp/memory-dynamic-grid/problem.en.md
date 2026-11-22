> A field is given,
> its rows and columns settled only on the day.
> Ask how many cells it holds,
> and where each one lies.

The Nine Chapters has an art: multiply the columns by the rows to obtain the field's extent; then from the first row onward, fill row by row. This is called row-major order.

Now there is a grid `Grid`, whose rows and columns are settled only at run time. Ask how it is built, how a cell is reached, how it is filled, and how it is copied out.

The art says: lay down one contiguous `std::vector<int>` of length `rows * cols`; the cell at row `r`, column `c` stands at position `r * cols + c`. `at(r, c)` takes that position directly, for every caller comes within the field and need not be questioned at the gate. `fill(v)` sets every cell to `v`; `row_major()` copies out all elements in row-major order, of length `rows * cols`. `rows()` and `cols()` report the dimensions given at construction.

And again the art says: a copy must have its own field, so that changes to the new field do not harm the old.

Do not make a `main`.

**In plain words**: implement `Grid(rows, cols)`. All elements live in one contiguous `std::vector<int>` in row-major order, with index `row * cols + col`. `rows()` and `cols()` return the dimensions given at construction. `at(row, col)` returns a reference; the indices are guaranteed valid. `fill(value)` sets every element to that value. `row_major()` returns a copy of the elements in row-major order, of length `rows * cols`. A copied grid must be independent of the original. Do not write `main`.

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
