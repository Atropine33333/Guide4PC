Read several floating-point values and format each one as a fixed-width line in scientific notation.

**Input**:

- The first line holds one integer `n` (`1 <= n <= 8`).
- The second line holds `n` floating-point values separated by single spaces; every value is finite and either `|x| <= 10^300` or `x = 0`.

**Output**:

- `n` lines, one per input value.
- Use scientific notation: one non-zero digit before the decimal point, exactly three digits after it, and a signed exponent with at least two digits.
- Right-align the whole line to width `12`, padding on the left with spaces.

For example, `1234` prints as `  1.234e+03` (12 characters, with three leading spaces).

### Example

Input:

```text
2
1234 0.03125
```

Output:

```text
  1.234e+03
  3.125e-02
```

### Constraints

- Use C++'s default scientific formatting; do not build the exponent yourself.
- `0` prints as `0.000e+00`; both positive and negative zero are treated as `0.000e+00`.
- Apart from the left padding, a line contains no other spaces.
