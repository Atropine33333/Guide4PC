这一题不读 stdin，也不写 stdout。实现一个函数，按“总是先拿面额最大的硬币”的规则清点金额，测试在隐藏的 harness 里。

```cpp
long long greedy_coin_count(long long amount);
```

规则：

- 硬币面额为 `200, 100, 50, 20, 10, 5, 2, 1`（单位：分）。
- `amount` 是非负整数，`0 ≤ amount ≤ 10^18`。
- 从左到右处理面额：每次取走尽可能多枚当前面额的硬币，再处理下一个面额。
- 返回取走的硬币总枚数。
- 不要写 `main`。

### 提示

对每个面额 `coin`，取 `amount / coin` 枚，然后 `amount %= coin`。总和也要用 `long long`：`10^18` 分全部用 200 分硬币需要 `5·10^15` 枚，`int` 存不下。
