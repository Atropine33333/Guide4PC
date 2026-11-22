这一题不读 stdin。实现三个函数，隐藏的 harness 会检查你对有符号/无符号转换和回绕的理解。

```cpp
bool unsigned_less(int a, unsigned b);
unsigned wrap_decrement(unsigned value, unsigned steps);
int countdown_loops(unsigned start);
```

要求：

- `unsigned_less(a, b)` 返回数学意义上的 `a < b`。注意 `a` 可能为负，而 `b` 可能大于 `INT_MAX`，所以不能直接写 `a < b`（那会把 `a` 转成 `unsigned`，负数变得很大）。
- `wrap_decrement(value, steps)` 返回按 32 位无符号取模的 `value - steps`，也就是结果落在 `[0, 2³²)` 内；下溢时从 `UINT_MAX` 继续。
- `countdown_loops(start)` 统计循环 `for (unsigned i = start; i > 0; --i) { ... }` 的循环体执行次数，`start` 不超过 `INT_MAX`，返回值可以放进 `int`。
- 不要写 `main`。

### 示例

| 调用 | 结果 |
| --- | --- |
| `unsigned_less(-1, 0)` | `true` |
| `unsigned_less(INT_MAX, 2147483648u)` | `true` |
| `wrap_decrement(0, 1)` | `4294967295` |
| `wrap_decrement(5, 3)` | `2` |
| `countdown_loops(5)` | `5` |
| `countdown_loops(0)` | `0` |

### 提示

判断有符号数与无符号数的大小时，先处理 `a < 0` 的情况，再统一转成无符号比较；无符号减法本身就按 2³² 取模，不需要手写分支。
