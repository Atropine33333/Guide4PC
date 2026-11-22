这一题不读 stdin，也不写 stdout。实现三个带溢出检查的整数运算函数，测试在隐藏的 harness 里。

```cpp
bool add_ints(int a, int b, int& out);
bool sub_ints(int a, int b, int& out);
bool mul_ints(int a, int b, int& out);
```

规则：

- 结果落在 `int` 范围内时，写入 `out` 并返回 `true`。
- 结果超出 `int` 范围时返回 `false`，且**不得修改** `out` 原来的值。
- 即使发生溢出，也不能让 `a + b`、`a - b`、`a * b` 直接以 `int` 计算，那会产生未定义行为。
- 不要写 `main`。

### 提示

把操作数转成 `long long` 再算。两个 `int` 的和、差、积一定落在 `long long` 范围内，所以可以安全地拿结果和 `int` 的上下界比较。
