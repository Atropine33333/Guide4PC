（深夜，演播室的灯光刺眼。主持人举起一只闪闪发光的桶。）

“朋友们，你是否厌倦了无休止的请求？看看这个——令牌桶！”

“构造的时候，桶是满的：`tokens()` 就是 `capacity`。零容量、零补充速率，我们也卖。”

“`advance(elapsed)` 一到，桶里就多出 `refill * elapsed` 个令牌；多到装不下？没关系，我们只装到桶口。”

“`take(elapsed)` 更妙：先补，再取。桶里有货，拿走一个，返回 `true`；桶里空空，返回 `false`。就这么简单！”

“容量为零？`take` 永远说 `false`。补充速率为零？令牌永远不涨。一分钱一分货。”

“数值大得吓人？`refill` 和 `elapsed` 都能到十亿。用 64 位整数，先乘，再和容量比大小。But wait, there's more——这一切只等你现在动手写！”

**人话**：补完 `TokenBucket`。用 `long long` 存令牌数，`advance` 先加 `1LL * refill_ * elapsed` 再与 `capacity_` 取较小值；`take` 先补后取。不要写 `main`。

```cpp
class TokenBucket {
public:
    TokenBucket(int capacity, int refill);
    bool take(int elapsed);
    void advance(int elapsed);
    int tokens() const;
    int capacity() const;
};
```

### 规则或约定

- 构造时桶是满的：`tokens() == capacity`；`capacity >= 0`，`refill >= 0`。
- `elapsed` 是距离上次操作经过的时间单位数，`elapsed >= 0`。
- `advance(elapsed)`：先给桶补充 `refill * elapsed` 个令牌，但总数不超过容量。
- `take(elapsed)`：先执行 `advance(elapsed)`，若桶里至少有一个令牌，消耗一个并返回 `true`；否则返回 `false`。
- 容量为 0 时 `take` 永远返回 `false`；补充速率为 0 时永远不会增加令牌。
- 补充数量可能很大（`refill` 和 `elapsed` 都可达到 `10^9`），计算时使用 64 位整数，先乘后截断到容量。
- 不要写 `main`。

### 提示

内部用 `long long` 保存当前令牌数。补充时先算 `tokens_ + 1LL * refill_ * elapsed`，再和 `capacity_` 取较小值。注意 `take` 先补充、后消耗。
