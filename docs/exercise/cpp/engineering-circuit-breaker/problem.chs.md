这一题不读 stdin，也不写 stdout。实现一个带冷却时间的熔断器，测试在隐藏的 harness 里。

```cpp
class CircuitBreaker {
public:
    enum class State { Closed, Open, HalfOpen };

    CircuitBreaker(int failure_threshold, int cooldown_ticks);
    bool allow() const;
    void record(bool success);
    void tick();
    State state() const;
    int failures() const;
};
```

规则：

- 初始为 `Closed`，失败计数为 0，`allow()` 返回 `true`。
- `failure_threshold >= 1`，`cooldown_ticks >= 0`。
- 在 `Closed` 状态下，`record(true)` 把失败计数清零并保持 `Closed`；`record(false)` 增加失败计数，达到阈值时熔断。
- 熔断后进入 `Open`（冷却时间为 0 时直接进入 `HalfOpen`），`allow()` 返回 `false`。
- 每次 `tick()` 在 `Open` 状态下把冷却计数减 1；减到 0 时进入 `HalfOpen`。
- `HalfOpen` 下 `allow()` 返回 `true`：`record(true)` 恢复正常，回到 `Closed` 且失败计数清零；`record(false)` 重新熔断。
- `Open` 状态下的 `record` 不做任何事。
- `tick()` 在 `Closed` 和 `HalfOpen` 状态下没有效果。
- 不要写 `main`。

### 示例

| 调用序列（阈值 3，冷却 2） | 状态 |
| --- | --- |
| 构造后 | `Closed`，失败 0 |
| `record(false)` 两次 | `Closed`，失败 2 |
| 第三次 `record(false)` | `Open`，`allow()` 为 `false` |
| `tick()` 两次 | `HalfOpen`，`allow()` 为 `true` |
| `record(true)` | `Closed`，失败 0 |

### 提示

用私有成员保存状态、连续失败次数、冷却剩余计数和两个构造参数。熔断逻辑可以抽成一个私有函数：`cooldown_ticks == 0` 时把状态设为 `HalfOpen`，否则设为 `Open` 并把剩余计数设为 `cooldown_ticks`。`tick()` 只在 `Open` 时递减，减到 0 后切到 `HalfOpen`。
