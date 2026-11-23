THE CODE OF QUOTIENT AND REMAINDER

Article 1 (Scope). Every question of the quotient and remainder of `a` divided by `b` is decided by this Code.

Article 2 (Truncation). The quotient `q` is `a / b`, truncated toward zero: exact when the division is exact; otherwise the fractional part is dropped, neither rounded nor rounded toward negative infinity.

Article 3 (Remainder). The remainder `r` is `a % b` and always satisfies `a = q * b + r` with `|r| < |b|`.

Article 4 (Sign). The sign of the remainder follows the dividend `a`: when `a` is positive, `r` is not negative; when `a` is negative, `r` is not positive. A negative divisor does not change this article.

Article 5 (Exact division). When `b` divides `a` exactly, `r` is `0`; zero carries no sign.

Article 6 (Smaller dividend). When `|a| < |b|`, `q` is `0` and `r` equals `a`.

Article 7 (Output). The quotient and the remainder go on one line, in that order, separated by a single space. `b` is never `0`.

**In plain words**: read two integers `a` and `b`, then print the quotient `a / b` and the remainder `a % b` under C's truncating division, separated by a single space.

**Input**: one line with two integers `a` and `b` such that `-10^9 ≤ a ≤ 10^9`, `-10^9 ≤ b ≤ 10^9`, and `b ≠ 0`.

**Output**: one line with two integers, the quotient then the remainder, separated by a single space.

### Example

Input:

```text
-7 3
```

Output:

```text
-2 -1
```

The quotient truncates toward zero to `-2`; since `-2 * 3 = -6`, the remainder is `-1`, sharing the sign of `-7`.

### Contract

- The quotient truncates toward zero; the remainder shares the dividend's sign.
- No trailing spaces at the end of the line.
- Both values fit in `int` and `b ≠ 0`, so the arithmetic cannot overflow.
- Print nothing beyond the answer.
