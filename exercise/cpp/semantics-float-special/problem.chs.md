《庄子·秋水（续）》

惠子谓庄子曰：「一尺之棰，日取其半，万世不竭。然则以有穷之器，量无穷之数，可乎？」

庄子曰：「可。器有定名，曰无穷，曰非数，曰负零。无穷者，大于一切有穷，加一仍为无穷；非数者，与万物皆不相等，虽其自身亦不相等；负零者，其值为零而符号为负，与正零相等，而符号可辨。」

惠子曰：「既不相等于自身，何以处之？」

庄子曰：「先问其有序无序。有序者，较其大小；无序者，归为一类，不复言先后。子欲知数，先知其器。」

**人话**：实现六个函数，处理 IEEE-754 的特殊值：识别 NaN、识别有限值、识别负零、用 `==` 比较、给出三路次序（无序时回 2）、以及分母为零时回落的安全除法。

```cpp
#include <cmath>

bool is_nan_value(double x);
bool is_finite_value(double x);
bool is_negative_zero(double x);
bool same_value(double a, double b);
int total_order(double a, double b);
double safe_div(double a, double b, double fallback);
```

### 约定

- `is_nan_value`：`x` 是 NaN 时为真。
- `is_finite_value`：`x` 是有限数（无穷大与 NaN 都不在其列）时为真。
- `is_negative_zero`：`x` 的值是零且符号位为负时为真（`-0.0` 为真，`+0.0` 为假）。
- `same_value`：用 `==` 比较；因此 `+0.0` 与 `-0.0` 相等，任何 NaN 都不等于任何值，包括它自己。
- `total_order`：`a < b` 返回 `-1`，`a > b` 返回 `1`，`a == b` 返回 `0`；只要有一个是 NaN（无序）就返回 `2`。
- `safe_div`：`b` 等于零（包括 `-0.0`）时返回 `fallback`，否则返回 `a / b`。`a` 和 `b` 可以是一般的有限数、无穷大或 NaN。
- 不要写 `main`。

### 提示

用 `<cmath>` 的 `std::isnan`、`std::isfinite` 和 `std::signbit`。NaN 与任何值比较都为假，所以 `total_order` 里四个判断的顺序很重要：先比大小，再比相等，最后落到无序。
