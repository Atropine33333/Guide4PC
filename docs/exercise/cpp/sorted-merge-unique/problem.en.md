Given two integer arrays sorted in non-decreasing order, merge them into one sorted array and remove duplicate values.

**Input**:

- The first line contains an integer `n` (`0 ≤ n ≤ 500`).
- The second line contains `n` integers in non-decreasing order; it is blank when `n = 0`.
- The third line contains an integer `m` (`0 ≤ m ≤ 500`).
- The fourth line contains `m` integers in non-decreasing order; it is blank when `m = 0`.

**Output**: one line with the merged, deduplicated integers in non-decreasing order, separated by single spaces. If the result is empty (both arrays empty), print `empty`.

### Example

Input:

```text
3
1 3 5
3
2 3 6
```

Output:

```text
1 2 3 5 6
```

### Hint

You can concatenate both arrays, sort the result, and call `std::unique`; or walk two indices like merge sort. Do not print a stray blank line for empty arrays.
