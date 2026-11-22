Implement packing and unpacking for binary-coded decimal (BCD).

BCD stores one decimal digit, `0` to `9`, in a nibble (4 bits). Two digits form one byte: the high nibble is the left digit and the low nibble is the right digit.

**Input**:

- The first line holds one character: `E` for encode, `D` for decode.
- For `E`: the second line holds a decimal digit string of length `1` to `20` (only `0`-`9`, possibly starting with `0`).
- For `D`: the second line holds space-separated two-digit uppercase hexadecimal bytes; every nibble is guaranteed to be a valid decimal digit.

**Output**:

- For `E`: one line of packed bytes, each as two uppercase hexadecimal digits, separated by one space.
  If the digit string has odd length, prepend a zero nibble to make it even.
- For `D`: one line with the decoded decimal digits, without leading zeros; print `0` for zero.

### Example

Input:

```text
E
123
```

Output:

```text
01 23
```

Input:

```text
D
00 99
```

Output:

```text
99
```

### Constraints

- The digit string has at most 20 characters, so encoding produces at most 10 bytes.
- Every decoded nibble is guaranteed to be in `0`-`9`.
- Decode by concatenating all nibbles first, then strip leading zeros once.
