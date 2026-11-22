Read an integer sequence and find its maximum subarray sum.

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 200000`).
- The second line contains `n` integers, each with absolute value at most `10^9`.

**Output**: one line with the maximum sum over all non-empty contiguous subarrays.

### Example

Input:

```text
7
1 -2 3 4 -1 2 -5
```

Output:

```text
8
```

The subarray `3 4 -1 2` sums to 8.

### Constraints

- The subarray must be non-empty; if all numbers are negative, the answer is the largest negative value, not 0.
- A subarray may contain up to 200000 values near `10^9`; the sum can exceed 32 bits, so use a 64-bit integer.
- An `O(n)` Kadane scan is required; do not enumerate all subarrays.
