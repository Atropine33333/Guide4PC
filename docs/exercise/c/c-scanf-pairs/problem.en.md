Read `n` pairs of integers and print the sum of each pair.

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 100`).
- The next `n` lines each contain two integers `a` and `b`, separated by one space. Either number may be negative, and its absolute value does not exceed `10^9`.

**Output**: `n` lines, each holding one integer: the sum `a + b` of the corresponding pair.

### Example

Input:

```text
3
2 3
-5 -7
0 12
```

Output:

```text
5
-12
12
```

### Contract

- Print exactly one sum per line, with no trailing spaces.
- The absolute value of a sum does not exceed `2 × 10^9`; use an integer type wide enough for that range.
- Output order matches the input order one to one.
- Do not merge equal sums, and do not skip any line.
