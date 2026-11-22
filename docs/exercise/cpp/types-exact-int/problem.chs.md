这一题不读 stdin，也不写 stdout。实现一个把 `double` 精确转换成 `int` 的函数，测试在隐藏的 harness 里。

```cpp
bool to_int_exact(double value, int& out);
```

规则：

- 当 `value` 是有限值、等于某个整数、且该整数落在 `int` 范围内（`-2147483648` 到 `2147483647`）时，把结果写入 `out` 并返回 `true`。
- 其他情况一律返回 `false`，且**不得修改** `out` 原来的值。
- `NaN`、正无穷、负无穷都返回 `false`；`-0.0` 可以转换，结果是 `0`。
- 不要写 `main`。

### 提示

先判断 `std::isfinite(value)`，再检查 `value == std::trunc(value)`，最后和 `int` 的上下界比较。不要先转换再检查：先把超范围的 `double` 转成 `int` 会产生未定义行为；`2147483648.0` 必须被拒绝。
