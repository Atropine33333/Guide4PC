An integer is left on the table to be copied three ways: hexadecimal, octal, and a binary written out one digit at a time.

**In plain words**: read a non-negative integer `n` and print its hexadecimal, octal and binary forms on three lines, with no prefix such as `0x` or `0` attached.

**Input**: one line with an integer `n`, `0 ≤ n ≤ 1000000000`.

**Output**: three lines.

- Line one: `n` in hexadecimal, lowercase letters, no prefix.
- Line two: `n` in octal, no prefix.
- Line three: `n` in binary, most significant bit first, no leading zeros; write `0` when `n` is `0`.

### Example

Input:

```text
45
```

Output:

```text
2d
55
101101
```

45 is `2d` in hexadecimal, `55` in octal, and `101101` in binary.

### Contract

- `n` fits in an `int`; the hexadecimal and octal forms can be written with `printf` conversion specifiers.
- Write the binary digits yourself, from the highest bit down, with no leading zeros.
- No trailing spaces at the end of a line, and nothing outside the requested output.
