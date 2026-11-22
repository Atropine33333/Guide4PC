Read `n` signed integers and report two kinds of averages:

- **truncating average**: the value of `sum / n` in C++, which rounds toward zero;
- **floor average**: the mathematical floor `⌊sum / n⌋`.

They agree when `sum` is divisible by `n`. They differ by one only when `sum` is negative and the division has a remainder.

**Input**: the first line contains an integer `n` (`1 ≤ n ≤ 1000`). The second line contains `n` integers, each with absolute value at most `10^9`.

**Output**: one line with two integers — the truncating average and the floor average — separated by a single space.

### Example

Input:

```text
2
-7 -8
```

Output:

```text
-7 -8
```

`-15 / 2` is `-7` in C++, while the floor is `-8`.

### Constraints

- Keep the total in a 64-bit integer.
- Do not compute the average with floating point.
