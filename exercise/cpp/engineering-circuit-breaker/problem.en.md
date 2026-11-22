This exercise reads no stdin and writes no stdout. Implement a circuit breaker with a cooldown, tested by a hidden harness.

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

Rules:

- It starts `Closed` with zero failures, and `allow()` returns `true`.
- `failure_threshold >= 1` and `cooldown_ticks >= 0`.
- In `Closed`, `record(true)` clears the failure count and stays `Closed`; `record(false)` increments it and trips the breaker when the threshold is reached.
- Tripping enters `Open` (or `HalfOpen` directly when the cooldown is 0), and `allow()` returns `false`.
- Each `tick()` decrements the cooldown while `Open`; when it reaches 0 the state becomes `HalfOpen`.
- In `HalfOpen`, `allow()` returns `true`: `record(true)` recovers to `Closed` with zero failures, and `record(false)` trips again.
- `record` does nothing while `Open`.
- `tick()` has no effect in `Closed` or `HalfOpen`.
- Do not write `main`.

### Example

| Call sequence (threshold 3, cooldown 2) | State |
| --- | --- |
| after construction | `Closed`, 0 failures |
| `record(false)` twice | `Closed`, 2 failures |
| a third `record(false)` | `Open`, `allow()` is `false` |
| `tick()` twice | `HalfOpen`, `allow()` is `true` |
| `record(true)` | `Closed`, 0 failures |

### Hint

Keep the state, the consecutive failure count, the remaining cooldown, and the two constructor arguments as private members. Factor the trip into a private helper: when `cooldown_ticks == 0` set the state to `HalfOpen`, otherwise set `Open` and the remaining count to `cooldown_ticks`. Only decrement in `tick()` while `Open`, switching to `HalfOpen` at 0.
