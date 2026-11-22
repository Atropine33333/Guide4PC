This exercise reads no stdin and writes no stdout. Implement a function that counts coins with the rule "always take the largest denomination that fits", tested by a hidden harness.

```cpp
long long greedy_coin_count(long long amount);
```

Rules:

- The denominations are `200, 100, 50, 20, 10, 5, 2, 1` (in cents).
- `amount` is a non-negative integer with `0 ≤ amount ≤ 10^18`.
- Process denominations from left to right: take as many coins of the current denomination as possible, then move to the next one.
- Return the total number of coins taken.
- Do not write `main`.

### Hint

For each denomination `coin`, take `amount / coin` coins and set `amount %= coin`. The total must be a `long long` as well: paying `10^18` cents with 200-cent coins needs `5·10^15` coins, which does not fit in `int`.
