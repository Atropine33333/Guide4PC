Read an integer between `0` and `255`, hand its eight bits unchanged to a `signed char`, and print the value that comes back.

**Input**: one line with an integer `n` such that `0 ≤ n ≤ 255`.

**Output**: one line with one integer: the value of `n`'s eight bits read as a signed 8-bit integer in two's complement.

### Example

Input:

```text
200
```

Output:

```text
-56
```

The eight bits of `200` are `11001000`; the top bit is `1`, so two's complement reads it as `200 - 256 = -56`.

### Contract

- `0` through `127` keep their value; `128` through `255` have `256` subtracted.
- Print in decimal, with a minus sign for negatives and no prefix.
- Print nothing beyond the answer.
