Read two integers `a` and `b`, then print the result of C++'s `%` operator together with a non-negative remainder.

**Input**: one line with two integers `a` and `b` (`b ≠ 0`, `|a| ≤ 10^9`, `|b| ≤ 10^9`).

**Output**: one line with two integers separated by a single space:

- The first is the value of the C++ expression `a % b`. When non-zero, its sign follows the dividend `a`.
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
