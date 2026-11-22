This exercise reads no stdin and writes no stdout. Implement an exponential backoff retry policy, tested by a hidden harness.

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

Rules:

- `base_delay >= 0`, `max_delay >= base_delay`, and `max_attempts >= 0`.
- The wait before attempt `k` (with `k` starting at 0) is `min(base_delay * 2^k, max_delay)`.
- `has_next()` returns `true` while `attempts() < max_attempts`.
- `next_delay()` returns the wait for the current attempt and increments `attempts()` by 1; when no attempts remain it returns `-1` and leaves `attempts()` unchanged.
- `reset()` sets the attempt counter back to zero.
- The computation must not overflow: with `base_delay` and `max_delay` near the `int` limit and many attempts, it must still return the correct capped value.
- Do not write `main`.

### Example

| Call sequence (`base=100, max=1000, attempts=5`) | Result |
| --- | --- |
| 1st `next_delay()` | `100` |
| 2nd | `200` |
| 3rd | `400` |
| 4th | `800` |
| 5th | `1000` |
| 6th | `-1`, and `attempts()` is still 5 |

### Hint

Keep `base_delay` in a `long long`, double it after each attempt, and take the smaller of that and `max_delay`. You can also stop doubling once the cap is reached to avoid overflow. In `next_delay`, check `has_next()` first, then compute `base * (1LL << attempts)`.
