这一题不读 stdin，也不写 stdout。实现一个指数退避的重试策略，测试在隐藏的 harness 里。

```cpp
class RetryPolicy {
public:
    RetryPolicy(int base_delay, int max_delay, int max_attempts);
    bool has_next() const;
    int next_delay();
    int attempts() const;
    void reset();
};
```

规则：

- `base_delay >= 0`，`max_delay >= base_delay`，`max_attempts >= 0`。
- 第 `k` 次尝试（`k` 从 0 开始）的等待时间是 `min(base_delay * 2^k, max_delay)`。
- `has_next()` 在 `attempts() < max_attempts` 时返回 `true`。
- `next_delay()` 返回当前这次尝试的等待时间，并把 `attempts()` 加 1；已经用完尝试次数时返回 `-1`，且不改变 `attempts()`。
- `reset()` 把尝试次数清零。
- 计算过程不能溢出：`base_delay` 和 `max_delay` 接近 `int` 上限、尝试次数较多时，仍要返回正确的上限值。
- 不要写 `main`。

### 示例

| 调用序列（`base=100, max=1000, attempts=5`） | 结果 |
| --- | --- |
| 第 1 次 `next_delay()` | `100` |
| 第 2 次 | `200` |
| 第 3 次 | `400` |
| 第 4 次 | `800` |
| 第 5 次 | `1000` |
| 第 6 次 | `-1`，且 `attempts()` 仍为 5 |

### 提示

用 `long long` 保存 `base_delay`，每次尝试后左移或乘以 2，再与 `max_delay` 取较小值。也可以在达到上限后停止倍增，避免溢出。`next_delay` 先检查 `has_next()`，再计算 `base * (1LL << attempts)`。
