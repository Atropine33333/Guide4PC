Read two integers `a` and `b` between 0 and 127, then watch what a `char` turns into once it joins an addition.

**In plain words**: cast `a` and `b` to `char`, add them, and print the integer value of the sum; then print the value of `sizeof(a + b)`; then print the value of `(unsigned char)300`. Put all three on one line, separated by single spaces.

**Input**: one line with two integers `a` and `b`, with `0 ≤ a, b ≤ 127`.

**Output**: one line with three integers:

1. the value of `(char)a + (char)b`;
2. the value of `sizeof(a + b)`;
3. the value of `(unsigned char)300`.

### Example

Input:

```text
100 50
```

Output:

```text
150 4 44
```

`(char)100` and `(char)50` are both promoted to `int` before the addition, so the sum is 150 and does not wrap inside 8 bits; the type of that sum expression is `int`, which takes 4 bytes here, so `sizeof` yields 4; `300` keeps only its low 8 bits in an `unsigned char`, and `300 - 256 = 44`.

### Contract

- Both operands stay between `0` and `127`, so the signedness of `char` does not matter.
- The result of `sizeof` may be cast to `int` before printing.
- On this machine `int` takes 4 bytes and `unsigned char` takes 1, as on the common platforms.
- No trailing spaces at the end of the line.
- Print nothing beyond the answer.
