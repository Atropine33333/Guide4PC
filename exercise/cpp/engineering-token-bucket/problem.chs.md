这一题不读 stdin，也不写 stdout。实现一个令牌桶限流器，测试在隐藏的 harness 里。

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

规则：

- 构造时桶是满的：`tokens() == capacity`；`capacity >= 0`，`refill >= 0`。
- `elapsed` 是距离上次操作经过的时间单位数，`elapsed >= 0`。
- `advance(elapsed)`：先给桶补充 `refill * elapsed` 个令牌，但总数不超过容量；然后返回。
- `take(elapsed)`：先执行 `advance(elapsed)`，若桶里至少有一个令牌，消耗一个并返回 `true`；否则返回 `false`。
- 容量为 0 时 `take` 永远返回 `false`；补充速率为 0 时永远不会增加令牌。
- 补充数量可能很大（`refill` 和 `elapsed` 都可达到 `10^9`），计算时使用 64 位整数，先乘后截断到容量。
- 不要写 `main`。

### 提示

内部用 `long long` 保存当前令牌数。补充时先算 `tokens_ + 1LL * refill_ * elapsed`，再和 `capacity_` 取较小值。注意 `take` 是先补充再消耗。
