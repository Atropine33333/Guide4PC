Given an integer array of length `n`, answer `q` range-sum queries.

**Input**:

- The first line contains one integer `n` (`1 ≤ n ≤ 200000`).
- The second line contains `n` integers, each with absolute value at most `10^9`.
- The third line contains one integer `q` (`1 ≤ q ≤ 200000`).
- The next `q` lines each contain two integers `l` and `r` (`1 ≤ l ≤ r ≤ n`).

**Output**: for each query, print one line with `a[l] + a[l+1] + … + a[r]`.

### Example

Input:

```text
5
1 2 3 4 5
3
1 5
2 4
3 3
```

Output:

```text
15
9
3
```

### Constraints

- Summing from scratch for every query is too slow; prefix sums answer each query in O(1).
- A range holds up to 200000 values of `10^9`, so the sum needs 64 bits.
