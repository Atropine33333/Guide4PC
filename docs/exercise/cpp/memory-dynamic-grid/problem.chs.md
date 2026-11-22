这一题不读 stdin。实现一个行数、列数在运行时决定的整数网格。

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

约定：

- 所有元素存放在**一块连续**的 `std::vector<int>` 中，按行优先排列：下标为 `row * cols + col`。
- `rows` / `cols` 返回构造时的维度。
- `at` 返回对应元素的引用；harness 只会传入合法下标，不需要做越界检查。
- `fill` 把所有元素设为 `value`。
- `row_major` 返回按行优先顺序排列的元素副本（长度 `rows * cols`）。
- 拷贝出来的网格必须与原网格互相独立（深拷贝）。
- 不要写 `main`。
