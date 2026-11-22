Given a sequence of positive integers and a target `t`, find the length of the shortest contiguous subarray whose sum is at least `t`.

**Input**:

- The first line holds two integers `n t` (`1 ≤ n ≤ 2·10^5`, `1 ≤ t ≤ 10^9`).
- The second line holds `n` positive integers `a[i]` (`1 ≤ a[i] ≤ 10^4`).

**Output**: one integer, the shortest length; print `0` when no such subarray exists.

### Example

Input:

```text
6 7
2 3 1 2 4 3
```

Output:

```text
2
```

Explanation: the subarray `[4, 3]` sums to 7 and has length 2.

### Constraints

- A subarray must be contiguous and non-empty.
- Every element is positive, so two pointers (a sliding window) work: extend the right end, shrink the left end, and each element enters and leaves the window at most once.
- The window sum needs a 64-bit integer; `n·a[i]` can reach `2·10^9`, beyond `int`.
