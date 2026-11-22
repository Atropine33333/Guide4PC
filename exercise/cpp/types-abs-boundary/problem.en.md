Read several 64-bit signed integers and print the absolute value of each on its own line.

Values are printed in decimal; for example, the absolute value of `-42` is `42`. Note that the absolute value of `long long`'s minimum, `-9223372036854775808`, is `9223372036854775808`, which is outside the positive range of `long long`.

**Input**:

- The first line holds one integer `n` (`1 <= n <= 100`).
- The second line holds `n` integers separated by single spaces, each within `[-2^63, 2^63 - 1]`.

**Output**:

- `n` lines, each a non-negative integer: the absolute value of the corresponding input.

### Example

Input:

```text
3
-7 0 9223372036854775807
```

Output:

```text
7
0
9223372036854775807
```

### Constraints

- The result is not guaranteed to fit in `long long`.
- Writing `-x` for the minimum value causes signed overflow, which is undefined behaviour.
- Print decimal digits without leading zeros or a plus sign.
