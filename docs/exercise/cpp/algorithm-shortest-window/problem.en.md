**House Rules**

At the far end of this corridor there is a window, and in the window lies a row of numbers. All who enter the corridor shall keep the following rules:

1. The window has two ends. Both the right end and the left end may move only to the right, and neither may turn back.
2. Each step of the right end adds that number to the window's sum; each step of the left end subtracts that number from it.
3. Whenever the window's sum is at least `t`, the left end must keep moving until the sum falls below `t`. Every width measured while the sum qualifies shall be recorded, and the smallest kept.
4. If the right end reaches the end of the corridor and the sum still falls short of `t`, the answer is recorded as `0`.
5. The window's sum shall be kept in a 64-bit integer. This rule stands under all circumstances.
Appendix: the rules above hold only while every number in the window is positive. Should a zero or a negative number appear, rule 3 is void, and another method must be sought.

**In plain words**: given a sequence of `n` positive integers and a target `t`, find the length of the shortest contiguous, non-empty subarray whose sum is at least `t`; print `0` when no such subarray exists. Use two pointers (a sliding window) and keep the sum in a 64-bit integer.

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
