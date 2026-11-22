这一题不读 stdin，也不写 stdout。实现一个判断两个 `double` 是否足够接近的函数，测试在隐藏的 harness 里。

```cpp
bool nearly_equal(double a, double b, double tolerance);
```

规则：

- `tolerance ≥ 0`。
- 两个数相等时返回 `true`；否则当 `|a - b| ≤ tolerance` 时返回 `true`。
- 同号的无穷大视为相等，返回 `true`；无穷大与有限数之间、正无穷与负无穷之间返回 `false`。
- 任一方是 NaN 时返回 `false`（即使两者都是 NaN）。
- 不要写 `main`。

### 提示

先判断 `a == b`，它能同时处理“有限数相等”和“同号无穷相等”。再判断 `std::isfinite`。如果直接写 `std::fabs(a - b) <= tolerance`，`inf - inf` 会得到 NaN，结果就变成 `false`。
