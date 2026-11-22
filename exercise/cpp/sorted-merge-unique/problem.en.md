**Chapter One**

And the LORD spake unto Moses, saying:

"Number two companies. The first company hath `n` men, the second `m`; let each stand in its own order, from few to many, and let none break rank. Thou shalt join the two companies into one, still from few to many; where the number is the same, keep one man only, and write no name twice. If both companies be empty, write `empty` in the roll."

And Moses did as the LORD commanded.

**In plain words**: Read two non-decreasing integer arrays, merge them into one non-decreasing array, and remove duplicate values; print one line with single spaces between neighbours. If both arrays are empty, print only `empty`. You may concatenate them, sort, and call `std::unique`, or advance two indices as in merge sort; do not print a stray blank line for empty arrays.

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
