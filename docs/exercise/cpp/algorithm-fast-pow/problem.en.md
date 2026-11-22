Huizi said to Zhuangzi: "There is a number, called `a`; and an exponent `b`, whose size no man knoweth—it may be thousands of li. Multiply `a` by itself `b` times, and the product is beyond counting. Yet we seek only its remainder upon division by `m`. How shall it be found?"

Zhuangzi said: "Hast thou not seen the rod? A rod of one foot, halved each day, is never exhausted in ten thousand ages. So it is with the exponent: halve it, and square the base, and halt when the halving is done. Why multiply one by one?"

Huizi said: "It is good."

**In plain words**: compute `a^b mod m`; `b` may be as large as `10^18`, so a step-by-step product is too slow—use binary exponentiation.

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

- The exponent can reach `10^18`; use binary exponentiation.
- For `b = 0` the result is `1 mod m`; for `a = 0` and `b > 0` it is `0`.
- The largest intermediate product is about `(10^9)^2 = 10^18`, so `long long` suffices; take the modulus after each multiplication.
