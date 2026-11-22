这一题不读 stdin，也不写 stdout。实现一个函数，返回每个长度为 `window` 的连续窗口中的最大值。

```cpp
#include <cstddef>
#include <vector>

std::vector<long long> sliding_max(const std::vector<long long>& values,
                                   std::size_t window);
```

规则：

- 结果长度应为 `values.size() - window + 1`（当 `1 <= window <= values.size()`）。
- 如果 `window == 0` 或 `window > values.size()`，返回空 `vector`。
- 窗口按从左到右的顺序排列，允许元素重复和负数。
- 期望时间复杂度为 `O(n)`；请使用单调双端队列，不要对每个窗口重新扫描。
- 不要写 `main`，不要修改函数签名。
