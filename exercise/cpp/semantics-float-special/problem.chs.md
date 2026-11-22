这一题不读 stdin。实现处理 IEEE-754 特殊值的函数，隐藏的 harness 会覆盖 NaN、无穷和有符号零。

```cpp
#include <cmath>

bool is_nan_value(double x);
bool is_finite_value(double x);
bool is_negative_zero(double x);
bool same_value(double a, double b);
int total_order(double a, double b);
double safe_div(double a, double b, double fallback);
```

约定：

- `is_nan_value`：`x` 是 NaN 时为真。
- `is_finite_value`：`x` 既不是无穷大也不是 NaN 时为真。
- `is_negative_zero`：`x` 的值是零且符号位为负时为真（`-0.0` 为真，`+0.0` 为假）。
- `same_value`：用 `==` 比较；因此 `+0.0` 与 `-0.0` 相等，任何 NaN 都不等于任何值，包括它自己。
- `total_order`：`a < b` 返回 `-1`，`a > b` 返回 `1`，`a == b` 返回 `0`；只要有一个是 NaN（无序）就返回 `2`。
- `safe_div`：`b` 等于零（包括 `-0.0`）时返回 `fallback`，否则返回 `a / b`。`a` 和 `b` 可以是一般的有限数、无穷大或 NaN。
- 不要写 `main`。

### 提示

用 `<cmath>` 的 `std::isnan`、`std::isfinite` 和 `std::signbit`。NaN 与任何值比较都为假，所以 `total_order` 里四个判断的顺序很重要：先比大小，再比相等，最后落到无序。
