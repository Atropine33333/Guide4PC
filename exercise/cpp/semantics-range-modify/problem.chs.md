实现三个处理整数序列的函数。

```cpp
#include <vector>

void scale_in_place(std::vector<int>& values, int factor);
long long sum_positive(const std::vector<int>& values);
int count_multiples(const std::vector<int>& values, int divisor);
```

规则：

- `scale_in_place` 把 `values` 中每个元素都乘以 `factor`，必须原地修改；`factor` 可以为 0 或负数。测试数据保证结果仍在 `int` 范围内。
- `sum_positive` 返回所有严格大于 0 的元素之和，结果是 `long long`；空序列返回 0。它接收 `const` 引用，不能修改序列。
- `count_multiples` 返回能被 `divisor` 整除（`value % divisor == 0`）的元素个数；`divisor` 保证非 0；空序列返回 0。
- 不要写 `main`。

### 提示

用 `for (int& value : values)` 而不是 `for (int value : values)`，后者修改的是副本。`sum_positive` 要把元素累加到 `long long` 变量里，避免多个 `int` 相加溢出。
