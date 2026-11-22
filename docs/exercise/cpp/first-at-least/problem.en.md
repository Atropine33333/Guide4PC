Given an array sorted in non-decreasing order, answer queries: at which position is the first element greater than or equal to a given value `x`?

**Input**:

- The first line contains one integer `n` (`1 ≤ n ≤ 200000`).
- The second line contains `n` integers in non-decreasing order, each with absolute value at most `10^9`.
- The third line contains one integer `q` (`1 ≤ q ≤ 200000`).
- The next `q` lines each contain one integer `x`, with absolute value at most `10^9`.

**Output**: one line per query. If the smallest `i` with `a[i] ≥ x` exists, print `i` (1-based); otherwise print `n + 1`.

### Example

Input:

```text
5
1 3 3 7 10
3
3
4
0
```

Output:

```text
2
4
1
```

### Hint

`std::lower_bound` does exactly this. It returns an iterator; subtract `begin()` and add 1. The array may contain duplicates, and binary search lands on the leftmost one.
