Given an integer sequence and a pivot value `k`, stably partition the sequence into three parts: values less than `k`, values equal to `k`, and values greater than `k`. Within each part the original relative order must be preserved.

### Input

- The first line holds `n` and `k` (`1 ≤ n ≤ 100000`, `-10^9 ≤ k ≤ 10^9`).
- The second line holds `n` integers, `-10^9 ≤ a[i] ≤ 10^9`.

### Output

One line with `n` integers separated by single spaces, with no trailing space.

### Example

Input:

```
6 10
5 12 3 10 7 10
```

Output:

```
5 3 7 10 10 12
```

### Constraints

- The partition must be stable: for `3 1 4 2 6 5` with `k = 3`, the values below 3 (`1 2`) come first in order, then the value equal to 3, then the values above 3 (`4 6 5`) in order.
- Standard library only.
