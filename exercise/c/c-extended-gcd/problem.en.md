> Two bands draw up on the plain,
> the less from the greater, again and again;
> when nothing remains, the captain stands clear,
> and the marshal's dispatch brings the ranks together.

In the rivers and lakes, the Jia band rides with `a` hands and the Yi band with `b`. They must form one patrol, every file with the same number of hands, and as many hands per file as possible: that number `g` is the greatest common divisor of the two bands' strengths. The old marshal is not satisfied with `g` alone; he sends a dispatch: let the Jia band furnish `x` companies and the Yi band `y` companies, so that the two parties together make exactly `g`. Only then is the patrol accounted complete.

**In plain words**: find the greatest common divisor `g` of `a` and `b`, and give one integer pair `x`, `y` with `a x + b y = g`. To make the answer unique, choose the `x` that satisfies `0 ≤ x < b / g`, then print `g x y` in that order.

**Input**: one line with two positive integers `a` and `b` (`1 ≤ a, b ≤ 10^9`), separated by a single space.

**Output**: one line with three integers `g`, `x`, `y`, separated by single spaces.

### Example

Input:

```text
12 8
```

Output:

```text
4 1 -1
```

The greatest common divisor of `12` and `8` is `4`; `12 × 1 + 8 × (-1) = 4`, and `1` lies in `0 ≤ x < 8 / 4 = 2`.

### Contract

- `g` is the greatest common divisor of `a` and `b`.
- `x` is the solution with `0 ≤ x < b / g`, and `y` follows from `(g - a x) / b`.
- The intermediate product `a x` stays within `10^18`, so store it in a `long long`.
- Both numbers are at least `1`, so there is no division by zero.
- No trailing spaces at the end of the line.
