Count how many times the decimal digit `d` occurs in all integers from 1 through `n`.

**Input**: two lines — the integer `n` (`0 ≤ n ≤ 10^17`), then the digit `d` (`0`–`9`).

**Output**: one integer, the number of occurrences. When `n = 0` the range is empty and the answer is `0`.

### Example

Input:

```text
20
1
```

Output:

```text
12
```

The digit `1` appears in 1, 10, 11, 12, …, 19: once in 1, once in 10, twice in 11, and once in each of 12–19, for a total of 12.

### Constraints

- Zero counts only in valid positions: the zeros in 10, 20 and 100 count, leading zeros do not.
- The count can exceed 32 bits; use a 64-bit integer.
- Work digit by digit in `O(log n)`; do not enumerate 1 through `n`.
