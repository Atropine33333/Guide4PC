Read a 32-bit unsigned integer `x`, split it into bytes, and print the byte-reversed result.

### Input

One line with a decimal integer `x` such that `0 <= x <= 4294967295`.

### Output

Two lines:

1. The four bytes of `x` from least significant to most significant, written in decimal and separated by single spaces.
2. The 32-bit integer obtained by reversing the order of those four bytes, written as `0x` followed by exactly eight lowercase hexadecimal digits (pad with leading zeros).

### Example

Input:

```text
16909060
```

Output:

```text
4 3 2 1
0x04030201
```

`16909060` is `0x01020304` in hexadecimal. Its least significant byte is `0x04` and its most significant byte is `0x01`; reversing the bytes gives `0x04030201`.

### Constraints

- The input always fits in a 32-bit unsigned integer.
- Use only the standard library; do not use platform-specific byte-order functions or union type punning.
- Print lowercase hexadecimal with exactly eight digits.
