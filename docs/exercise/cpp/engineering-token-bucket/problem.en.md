(Late night. Studio lights glare. The host lifts a gleaming bucket.)

"Friends, are you tired of endless requests? Look at this — the token bucket!"

"At construction the bucket is full: `tokens()` equals `capacity`. Zero capacity? Zero refill? We sell those too."

"When `advance(elapsed)` comes around, the bucket gains `refill * elapsed` tokens. Too many to hold? No problem — we fill it only to the brim."

"`take(elapsed)` is even better: refill first, then take. Got a token? Take one and return `true`. Empty? Return `false`. That simple!"

"Capacity zero? `take` always says `false`. Refill zero? Tokens never grow. You get exactly what you pay for."

"Big numbers? `refill` and `elapsed` both reach a billion. Use 64-bit integers, multiply first, then compare with the capacity. But wait, there's more — all this can be yours the moment you write it!"

**In plain words**: Complete `TokenBucket`. Store the token count in `long long`; `advance` adds `1LL * refill_ * elapsed` and caps at `capacity_`; `take` refills before consuming. Do not write `main`.

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

### Rules

- The bucket starts full: `tokens() == capacity`; `capacity >= 0` and `refill >= 0`.
- `elapsed` is the number of time units since the previous operation, `elapsed >= 0`.
- `advance(elapsed)`: add `refill * elapsed` tokens, capped at the capacity, then return.
- `take(elapsed)`: first call `advance(elapsed)`, then consume one token and return `true` if at least one is available; otherwise return `false`.
- With capacity 0, `take` always returns `false`; with refill rate 0, tokens never increase.
- Refill amounts can be large (`refill` and `elapsed` may both reach `10^9`); compute in 64 bits, multiply first, then cap at the capacity.
- Do not write `main`.

### Hint

Store the token count as `long long`. When refilling, compute `tokens_ + 1LL * refill_ * elapsed` and take the smaller of that and `capacity_`. Note that `take` refills before consuming.
