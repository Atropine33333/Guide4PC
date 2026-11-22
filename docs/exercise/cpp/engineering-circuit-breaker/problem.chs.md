欢迎来到熔断走廊。

这里的每一条规则都是真的，走廊不骗人。门卫只认三块牌子：`Closed`、`Open`、`HalfOpen`。

规则一：刚进门时门是通的，牌子写 `Closed`，失败计数为零，`allow()` 返回 `true`。

规则二：门开着的时候，一次平安无事让计数归零，一次出事让计数加一；加满阈值，门就合上。

规则三：合上的门分两种。冷却为零没有等候区，直接挂 `HalfOpen`；冷却大于零挂 `Open`，`allow()` 返回 `false`。

规则四：`Open` 的门只认 `tick()`，敲一次剩余冷却少一，归零换成 `HalfOpen`。`record` 在这扇门后面喊什么都不作数。

规则五：`HalfOpen` 是试用期，`allow()` 重新返回 `true`；平安无事回到 `Closed` 并清零，再出一次事门重新合上。

规则六：在 `Closed` 与 `HalfOpen` 里敲 `tick()` 没有回声。不要写 `main`。

**人话**：补完 `CircuitBreaker`。构造参数为失败阈值与冷却次数；`record` 推动失败计数，`tick()` 只在 `Open` 时递减冷却。隐藏的 harness 逐条核对 `state()`、`failures()` 与 `allow()`。

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

### 规则或约定

- `failure_threshold >= 1`，`cooldown_ticks >= 0`。
- `Closed` 下 `record(true)` 清零失败计数并保持 `Closed`；`record(false)` 增加计数，达到阈值时熔断。
- 熔断进入 `Open`；`cooldown_ticks == 0` 时直接进入 `HalfOpen`。
- `tick()` 只在 `Open` 时递减剩余冷却，减到 0 后进入 `HalfOpen`；在 `Closed` 与 `HalfOpen` 下没有效果。
- `HalfOpen` 下 `record(true)` 恢复 `Closed` 并清零失败计数；`record(false)` 重新熔断。
- `Open` 状态下的 `record` 不改变任何状态。
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
