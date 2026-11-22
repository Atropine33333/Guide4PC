Welcome to the Breaker Corridor.

Every rule here is true; the corridor does not lie. The doorman recognises three signs only: `Closed`, `Open`, `HalfOpen`.

Rule one: on arrival the door is open. The sign reads `Closed`, the failure count is zero, and `allow()` returns `true`.

Rule two: while the door is open, one clean pass clears the count, and one failure raises it by one. When it reaches the threshold, the door shuts.

Rule three: a shut door takes two forms. With a cooldown of zero there is no waiting room, so the sign goes straight to `HalfOpen`; with a cooldown above zero the sign reads `Open` and `allow()` returns `false`.

Rule four: an `Open` door listens only to `tick()`. Each knock lowers the remaining cooldown by one, and at zero the sign becomes `HalfOpen`. Whatever `record` shouts behind that door does not count.

Rule five: `HalfOpen` is a trial. `allow()` says `true` again; a clean pass returns to `Closed` with the count cleared, and one more failure shuts the door.

Rule six: `tick()` in `Closed` or `HalfOpen` has no echo. Do not write `main`.

**In plain words**: Complete `CircuitBreaker`. The constructor takes the failure threshold and the cooldown length; `record` drives the failure count, and `tick()` lowers the cooldown only while `Open`. A hidden harness checks `state()`, `failures()`, and `allow()`.

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

### Rules

- `failure_threshold >= 1` and `cooldown_ticks >= 0`.
- In `Closed`, `record(true)` clears the failure count and stays `Closed`; `record(false)` increments it and trips the breaker at the threshold.
- Tripping enters `Open`; when `cooldown_ticks == 0` it enters `HalfOpen` directly.
- `tick()` decrements the remaining cooldown only while `Open`, entering `HalfOpen` at 0; it has no effect in `Closed` or `HalfOpen`.
- In `HalfOpen`, `record(true)` recovers to `Closed` with zero failures, and `record(false)` trips again.
- `record` does nothing while `Open`.
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
