这一题不读 stdin。实现几个不会产生未定义行为的饱和转换函数。

```cpp
#include <cmath>
#include <limits>

int saturate_to_int(long long x);
short saturate_to_short(long long x);
int saturate_double_to_int(double x);
```

约定：

- `saturate_to_int`：把 `x` 截到 `int` 的范围 `[INT_MIN, INT_MAX]`；在范围内时原样返回。
- `saturate_to_short`：把 `x` 截到 `short` 的范围 `[-32768, 32767]`；在范围内时原样返回。
- `saturate_double_to_int`：
  - `x` 是 NaN 时返回 `0`；
  - `x` 是正无穷时返回 `INT_MAX`，负无穷时返回 `INT_MIN`；
  - 其他情况下先向零截断成整数，再截到 `int` 范围。

要求：

- 不能先直接把 `long long` 或 `double` 强转成较窄类型再判断，那正是未定义行为发生的地方。
- 使用 `std::numeric_limits` 和 `<cmath>` 的判断，不要写死平台相关的魔数（`short` 至少 16 位，本题按常见的 16 位 `short` 和 32 位 `int` 检查）。
- 不要写 `main`。

### 提示

先把边界转换成更宽的类型再比较，或者直接与 `std::numeric_limits<T>::max()` 比较。处理 `double` 时，`static_cast<double>(INT_MAX)` 是可以精确表示的安全上界。
