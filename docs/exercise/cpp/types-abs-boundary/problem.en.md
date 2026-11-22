Notes on the Use of Absolute Values

1. These notes apply to every 64-bit signed integer.
2. For any `x`, its absolute value is written `|x|` and printed in decimal, with no leading zeros and no plus sign. For example, the absolute value of `-42` is `42`.
3. The minimum of `long long` is `-9223372036854775808`, and its absolute value is `9223372036854775808`.
4. Rule 3 is true at all times. That number is one larger than the greatest positive value `long long` can represent.
5. Consequently, writing `-x` for the minimum triggers signed integer overflow and is undefined behaviour; rule 2 still applies there.
6. If you assume the result always fits back into `long long`, rule 2 is void for you: what you hand back will be a negative number, or worse.
7. These notes exist only to state that all of the above hold at once. Choosing a container is not their business.

**In plain words**: read `n` 64-bit signed integers and print each one's absolute value on its own line. The result may exceed the positive range of `long long`, so use either an unsigned 64-bit integer or treat the decimal digits as a string.

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
