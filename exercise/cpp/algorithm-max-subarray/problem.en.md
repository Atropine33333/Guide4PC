Given a sequence of integers, you may delete at most one element (or none), then take a non-empty contiguous subarray of the result and maximize its sum.

That single deletion is usually spent on the one value inside the window that drags it down the most.

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 200000`).
- The second line contains `n` integers, each with absolute value at most `10^9`.

**Output**: one line with the largest possible subarray sum.

### Example

Input:

```text
7
1 -2 3 4 -1 2 -5
```

Output:

```text
9
```

Take the subarray `3 4 -1 2`, delete `-1`, and the remaining `3 4 2` sums to 9.

### Constraints

- At most one element may be deleted; when `n = 1`, nothing can be deleted.
- The subarray must be non-empty; if all numbers are negative, the answer is still the largest single value.
- A subarray may hold up to 200000 values near `10^9`, so the sum can exceed 32 bits; use a 64-bit integer.
- An `O(n)` scan is required: track two states ending at the current position, one with no deletion and one with the deletion already used.
