欢迎来到无符号走廊。

这里的每一条规则都是真的。走廊不骗人。你只需要决定：还要不要相信你带进来的那套常识。

规则一：在走廊里，任何数都不会小于零。你看见的 `-1`，是 4294967295 换了一件旧外套。请把它当成那个很大的数。

规则二：常识说，从 0 再往回走一步，会走到 -1。走廊里没有 -1。`wrap_decrement(value, steps)` 从 `value` 往回走 `steps` 步，越过 0 之后会从 4294967295 那一端重新出现。

规则三：常识说，倒着数总会数到 0，然后停下。`countdown_loops(start)` 里只有一个刹车：`i > 0`。当 `start` 为 0 时，本条成立：刹车从一开始就踩着，一圈也不会转。

规则四：`unsigned_less(a, b)` 比较两个数之前，先看左边那个的符号。若 `a` 为负，答案为真，比较就此结束；此时规则五不适用。

规则五：若 `a` 非负，规则四的符号判断作废。请把 `a` 与 `b` 都放进无符号世界，再按大小排。

规则六：走廊的门从外面开。不要写 `main`。

如果你发现自己开始觉得这些规则很正常，请回到开头，重读第一句。

**人话**：实现三个函数：有符号数与无符号数的安全比较、无符号减法的回绕、无符号倒计数循环的执行次数。`start` 不超过 `INT_MAX`，返回值可以放进 `int`。

```cpp
bool unsigned_less(int a, unsigned b);
unsigned wrap_decrement(unsigned value, unsigned steps);
int countdown_loops(unsigned start);
```

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

比较有符号数与无符号数时，先处理 `a < 0` 的情况，再统一转成无符号比较；无符号减法本身就按 2³² 取模，不需要手写分支。
