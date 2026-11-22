Given an integer sequence and a series of position swaps, output the sequence after all swaps.

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 200`).
- The second line contains `n` integers with absolute value at most `10^9`.
- The third line contains an integer `m` (`0 ≤ m ≤ 200`).
- The next `m` lines each contain two integers `i` and `j` (`1 ≤ i, j ≤ n`), meaning the `i`-th and `j`-th elements should be swapped.

**Output**: one line with the final `n` integers, separated by single spaces.

### Example

Input:

```text
5
1 2 3 4 5
2
1 5
2 4
```

Output:

```text
5 4 3 2 1
```

### Hint

The starter already implements input, the calls and output; only the swap function is missing. If its parameters are passed by value, the function swaps copies and the caller's array stays unchanged.
