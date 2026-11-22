有符号整数运算法

第一条 本法适用于 `int` 上的加、减、乘三项运算，三项各设一函数，其式如左：

```cpp
bool add_ints(int a, int b, int& out);
bool sub_ints(int a, int b, int& out);
bool mul_ints(int a, int b, int& out);
```

第二条 结果落在 `int` 范围内者，写入 `out`，返回 `true`。
第三条 结果超出 `int` 范围者，返回 `false`，`out` 保持原值，一字不得更动。
第四条 凡以 `int` 直接计算 `a + b`、`a - b`、`a * b` 而可能溢出者，即为未定义行为，本法概不承认其效力。
第五条 审查须在运算之前定夺，不得先算后查。先算后查者，溢出已成事实，虽欲救之，无及矣。
第六条 不得书写 `main`。

附则 可将操作数转为 `long long` 再算。两个 `int` 的和、差、积必落于 `long long` 之内，故可安全地与 `int` 上下界相较；比较之后再决定写不写入 `out`。

**人话**：实现三个带溢出检查的运算函数。结果在 `int` 内就写入 `out` 返回 `true`；溢出就返回 `false` 且不碰 `out`。
