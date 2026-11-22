Read an integer sequence and a closed interval `[lo, hi]`. Keep only the elements inside the interval, delete the rest, and preserve the original order.

**Input**:

- The first line holds three integers `n lo hi` (`0 ≤ n ≤ 1000`, `|lo|, |hi| ≤ 10^9`).
- The second line holds `n` integers; when `n = 0` this line may be empty.

**Output**:

- The first line holds one integer `k`, the number of kept elements.
- When `k > 0`, the second line holds those `k` elements separated by single spaces; when `k = 0`, print only the first line.

### Example

Input:

```text
5 10 20
9 10 15 20 21
```

Output:

```text
3
10 15 20
```

### Constraints

- The interval is closed: both `lo` and `hi` are kept.
- `lo` may be greater than `hi`; then every element is deleted.
- Do not print a trailing space.
