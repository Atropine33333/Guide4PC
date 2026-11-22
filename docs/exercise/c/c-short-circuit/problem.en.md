Two gates stand side by side at the crossing; the left one is marked `&&`, the right one `||`.

The guard at `&&` is lazy: if the person on the left fails, he does not even question the one behind, and turns everyone away. The guard at `||` is just as lazy: if the person on the left passes, he does not question the one behind either, and lets everyone through.

Two numbers `a` and `b` arrive, and two things must be decided: `b` is nonzero and the quotient `a / b` is greater than `2`; `b` is zero or the quotient `a / b` is greater than `2`. If the division were really performed with a zero divisor, the program would fall over; thanks to the lazy guards, that question never comes up.

**In plain words**: read `a` and `b`, then print the truth values of `(b != 0 && a / b > 2)` and `(b == 0 || a / b > 2)`, writing `1` for true and `0` for false, separated by a single space. `b` may be `0`.

**Input**: one line with two integers `a` and `b` such that `-10^9 ≤ a ≤ 10^9` and `-10^9 ≤ b ≤ 10^9`.

**Output**: one line with two values, `0` or `1`, for the first and then the second expression.

### Example

Input:

```text
10 2
```

Output:

```text
1 1
```

`10 / 2 = 5`, which is greater than `2`, so both expressions are true.

Another:

```text
10 0
```

Output:

```text
0 1
```

`&&` stops at `b != 0`, `||` stops at `b == 0`, and neither ever reaches the division.

### Contract

- Write the two expressions directly and let the evaluation order of `&&` and `||` avoid the division by zero; do not branch on `b` with `if` first.
- The quotient truncates toward zero, both values fit in `int`, and `a / b` cannot overflow.
- No trailing spaces at the end of the line.
