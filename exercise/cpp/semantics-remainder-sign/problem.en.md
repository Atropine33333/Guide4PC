THE NINE CHAPTERS ON THE MATHEMATICAL ART — REMAINDERS

Suppose there are two numbers, A the dividend and B the divisor, and B is not zero. It is asked: what is the remainder after the manner of C++? And what value brings that remainder to lie between zero and the absolute value of B?

Method: divide A by B, truncating the quotient toward zero; multiply the quotient by B and subtract the product from A; what is left is the remainder. Its sign follows A, as this method provides. Should the remainder be negative, add the absolute value of B, and add it again until the result is no longer negative; the result is then surely less than the absolute value of B, and A minus it is divisible by B without remainder.

Answer: write the truncated remainder first, then the corrected remainder, with one space between them.

**In plain words**: read two integers `a` and `b`, then print two things separated by a single space: the value of the C++ expression `a % b`, and the remainder `r` normalized to `0 ≤ r < |b|`. `b` may be negative but is never zero; stay in integer arithmetic and keep floating point out of it.

### Input

One line with two integers `a` and `b` (`b ≠ 0`, `|a| ≤ 10^9`, `|b| ≤ 10^9`).

### Output

One line with two integers:

- The first is the value of the C++ expression `a % b`. When the value is non-zero, its sign follows the dividend `a`.
- The second is the remainder `r` satisfying `0 ≤ r < |b|` and `a - r` divisible by `b`.

### Example

Input:

```text
-7 3
```

Output:

```text
-1 2
```

`-7 / 3` truncates toward zero and gives `-2`, so `-7 % 3` is `-1`; reaching the non-negative range needs one more `|b|`.

### Constraints

- `b` may be negative but is never 0.
- Do not use floating point to compute the remainder.
