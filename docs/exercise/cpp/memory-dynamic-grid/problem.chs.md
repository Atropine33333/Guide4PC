> 今有方田一区，
> 行数、列数临时而定。
> 问田中共若干格，
> 每格所直几何？

《九章》有术：以列数乘行数，得田之积；自首行起，逐行铺满，谓之行优先。

今有网格 `Grid`，行数 `rows`、列数 `cols` 皆临时而定，问其构造、存取、填值与抄录之法。

术曰：置一段首尾相连之 `std::vector<int>`，长 `rows * cols`；第 `r` 行第 `c` 列，居 `r * cols + c` 之位。`at(r, c)` 直取此位，取者皆合于田，不劳问界。`fill(v)` 遍置诸格为 `v`；`row_major()` 依行优先之序抄出全部元素，长 `rows * cols`。`rows()`、`cols()` 报初造之数。

又术曰：抄录之作，须各具一格，新田之变不害旧田。

勿作 `main`。

**人话**：实现 `Grid(rows, cols)`：所有元素放在一块连续的 `std::vector<int>` 里，按行优先排列，下标为 `row * cols + col`；`rows()` / `cols()` 返回构造时的维度；`at(row, col)` 返回引用，harness 只传合法下标；`fill(value)` 把所有元素设为该值；`row_major()` 返回按行优先顺序排列的元素副本，长度 `rows * cols`；拷贝出来的网格必须与原网格互相独立。本题不读 stdin；不写 `main`。

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
