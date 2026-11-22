Compute `a` raised to the power `b`, modulo `m`.

**Input**: one line with three integers `a b m` (`0 ≤ a ≤ 10^9`, `0 ≤ b ≤ 10^18`, `1 ≤ m ≤ 10^9`).

**Output**: one integer, `a^b mod m`.

### Example

Input:

```text
2 10 1000
```

Output:

```text
24
```

### Constraints

- The exponent can reach `10^18`; multiplying one factor at a time is too slow, so use binary exponentiation.
- When `b = 0` the result is `1 mod m`; when `a = 0` and `b > 0` the result is 0.
- An intermediate product is at most about `(10^9)^2 = 10^18`, which fits in a `long long`; reduce modulo `m` after every multiplication.
