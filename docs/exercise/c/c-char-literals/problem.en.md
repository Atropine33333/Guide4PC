A character constant in C is just an integer written to look like a character. `'A'` is `65`; `'\n'` is the newline code `10`; `'\x41'` spells the same `A` in hexadecimal; `'\\'` is a backslash, `92`; `'0'` is the character zero, `48`.

**In plain words**: read `k`, then print one line of five integers: the values of `'A' + k`, `'\n'`, `'\x41'`, `'\\'`, and `'0'`.

**Input**: one line with a single integer `k` (`0 ≤ k ≤ 25`).

**Output**: one line with five integers separated by single spaces.

### Example

Input:

```text
2
```

Output:

```text
67 10 65 92 48
```

### Contract

- Print all five values with `%d`.
- `'A' + k` has type `int`; for `k` from `0` to `25` the result lies between `A` and `Z`.
- `'\x41'` has the same value as `'A'`, while `'\\'` is the backslash itself.
- No trailing spaces at the end of the line.
