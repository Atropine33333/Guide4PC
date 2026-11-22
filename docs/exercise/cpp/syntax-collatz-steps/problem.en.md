Read a positive integer `n` and repeatedly apply the Collatz rule: divide even values by 2, and replace odd values with `3n + 1`, until the value becomes 1. Output the number of transformations needed and the largest value seen along the way (including the starting `n` and the final 1).

**Input**: one integer `n` (`1 ≤ n ≤ 10^6`).

**Output**: two integers on one line, separated by a single space: the number of transformations and the maximum value.

### Example

Input:

```text
6
```

Output:

```text
8 16
```

The sequence is `6 → 3 → 10 → 5 → 16 → 8 → 4 → 2 → 1`: 8 transformations, maximum 16.

### Constraints

- For `n = 1` no transformation is needed: the count is 0 and the maximum is 1.
- Intermediate values may exceed the 32-bit integer range; store them in a 64-bit signed integer.
- The output contains exactly two decimal integers and one space.
