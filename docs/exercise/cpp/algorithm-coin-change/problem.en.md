Given a set of coin denominations and a target amount, find the minimum number of coins that sum exactly to the target. Every denomination may be used any number of times.

**Input**:

- The first line holds two integers `m` and `target` (`1 <= m <= 10`, `0 <= target <= 5000`).
- The second line holds `m` distinct positive integers, the coin denominations, each at most `1000`.

**Output**:

- One integer: the minimum coin count, or `-1` if the target cannot be formed exactly.
- For `target = 0`, print `0`.

### Example

Input:

```text
3 6
1 3 4
```

Output:

```text
2
```

Two coins of value `3`; a greedy choice of `4` gives `4 + 1 + 1`, which needs three coins.

### Constraints

- Every denomination is available in unlimited supply.
- Denominations are distinct but appear in arbitrary order.
- Only the minimum count matters; no reconstruction is required.
- Print `-1` when the target is unreachable; do not print `0` or a large sentinel.
