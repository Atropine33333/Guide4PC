A 32-bit unsigned integer can be packed into four bytes in two orders: little-endian puts the least significant byte first, big-endian puts the most significant byte first. This task packs little-endian: read a decimal unsigned integer, split it into four bytes with shifts and masks, and print them from low to high, each as two uppercase hexadecimal digits.

**Input**: one line with a decimal unsigned integer `n`, where `0 ≤ n ≤ 4294967295`.

**Output**: one line with four two-digit uppercase hexadecimal bytes in little-endian order, separated by single spaces. The least significant byte comes first and the most significant byte comes last.

### Example

Input:

```text
305419896
```

Output:

```text
78 56 34 12
```

`305419896` is `0x12345678`: the lowest byte `0x78` comes first and the highest byte `0x12` comes last.

### Contract

- Store the value in a `uint32_t` and build the `scanf` format string with the `SCNu32` macro.
- Output byte `i` (`i` from `0` to `3`) is `(value >> (8 * i)) & 0xFF`.
- Print each byte with `%02X`, at least two digits, using uppercase letters.
- Pack explicitly by the little-endian rule, so the result does not depend on whether this machine is big-endian or little-endian; do not read raw bytes through a pointer.
- No trailing spaces at the end of the line.
