This exercise reads no stdin and writes no stdout. Implement a token bucket rate limiter; a hidden harness runs the tests.

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

Rules:

- The bucket starts full: `tokens() == capacity`; `capacity >= 0` and `refill >= 0`.
- `elapsed` is the number of time units since the previous operation, `elapsed >= 0`.
- `advance(elapsed)`: add `refill * elapsed` tokens, capped at the capacity, then return.
- `take(elapsed)`: first call `advance(elapsed)`, then consume one token and return `true` if at least one is available; otherwise return `false`.
- With capacity 0, `take` always returns `false`; with refill rate 0, tokens never increase.
- Refill amounts can be large (`refill` and `elapsed` may both reach `10^9`); compute in 64 bits, multiply first, then cap at the capacity.
- Do not write `main`.

### Hint

Store the token count as `long long`. When refilling, compute `tokens_ + 1LL * refill_ * elapsed` and take the smaller of that and `capacity_`. Note that `take` refills before consuming.
