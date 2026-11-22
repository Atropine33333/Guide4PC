Now hear this. There was a number by the name of Collatz, and an odd temper he had.

Meet an even number and he halves it; meet an odd number and he trebles it and adds one. On and on he goes, and whatever the start, he comes down to 1 in the end. No one in the trade has proved why, and no one has yet found a case that escapes him.

Good people, today we do not ask why he comes down; we ask how long he took and how high he climbed on the way. The starting number counts as part of the road, and the final 1 counts too.

**In plain words**: read a positive integer `n`, apply the Collatz rule until the value reaches 1, and print the number of transformations and the largest value seen along the way (the starting `n` and the final 1 included), separated by one space.

**Input**: one integer `n` on a line (`1 ≤ n ≤ 10^6`).

**Output**: two integers on one line, separated by a single space: the transformation count and the maximum value.

### Example

Input:

```text
6
```

Output:

```text
8 16
```

The sequence is `6 → 3 → 10 → 5 → 16 → 8 → 4 → 2 → 1`: eight transformations, maximum 16.

### Constraints

- For `n = 1` no transformation is needed: count 0, maximum 1.
- Intermediate values may pass beyond the 32-bit integer range; keep them in a 64-bit signed integer.
- The output holds exactly two decimal integers and one space.
