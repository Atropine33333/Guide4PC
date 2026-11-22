实现两个对 `std::set<int>` 的区间查询函数。

```cpp
#include <cstddef>
#include <set>
#include <vector>

std::vector<int> window_values(const std::set<int>& values, int low, int high);

std::size_t window_count(const std::set<int>& values, int low, int high);
```

规则：

- `window_values` 返回集合中满足 `low <= x <= high` 的所有元素，按升序排列。
- `window_count` 返回满足同样条件的元素个数，不构造结果数组。
- 区间是闭区间，`low` 和 `high` 本身都包含在内。
- 如果 `low > high`，或者区间内没有元素，返回空 `vector` / `0`。
- 建议使用 `lower_bound` 和 `upper_bound`，这样查询不需要扫描整个集合。
- 不要写 `main`，不要修改函数签名。
