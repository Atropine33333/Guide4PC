For every position in a sequence, find the first element to its right that is strictly greater.

**Input**:

- The first line holds an integer `n` (`1 ≤ n ≤ 10^5`).
- The second line holds `n` integers `a[i]` (`|a[i]| ≤ 10^9`).

**Output**: one line with `n` integers; the `i`-th is the first element right of `a[i]` that is strictly greater than `a[i]`, or `-1` when none exists. Separate adjacent integers with one space.

### Example

Input:

```text
5
2 1 2 4 3
```

Output:

```text
4 2 4 -1 -1
```

### Constraints

- "Strictly greater" means equal values do not count: a sequence of equal `3`s prints only `-1`.
- Scan right to left and keep a monotonic stack of candidate indices that have not found a greater element yet; each element enters and leaves the stack at most once.
- A naive double loop is correct but may time out when `n = 10^5`.
