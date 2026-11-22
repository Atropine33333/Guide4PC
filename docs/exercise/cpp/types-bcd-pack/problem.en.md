And the Lord spoke unto Moses, saying:

"Number the people by nibbles, two to a row: let the one on the left take the high place and the one on the right the low place, and the two shall become one byte. If the count is odd, set an empty place, `0`, before them all, that they may be paired. And write each row as two marks, separated by a space."

And Moses numbered them. And on the day the roll was opened, the Lord said further: "Read out the two of each row in order and join them into one long string, then wipe away the surplus zeros from the head; and if none remain, write a single `0`. The digits you read shall run only from `0` to `9`; every nibble on the roll is guaranteed to be a lawful digit."

**In plain words**: implement BCD packing and unpacking. `E` encodes: pair up the decimal digits into bytes, one decimal digit per nibble (4 bits), high nibble first, prefix a `0` nibble when the length is odd, and print each byte as two uppercase hexadecimal digits. `D` decodes: split the hexadecimal bytes into nibbles, join them into one digit string, strip leading zeros, and print `0` when everything is zero.

**Input**:

- The first line holds one character: `E` for encode, `D` for decode.
- For `E`: the second line holds a decimal digit string of length `1` to `20` (only `0`-`9`, possibly starting with `0`).
- For `D`: the second line holds space-separated two-digit uppercase hexadecimal bytes; every nibble is guaranteed to be a valid decimal digit.

**Output**:

- For `E`: one line of packed bytes, each as two uppercase hexadecimal digits, separated by one space. If the digit string has odd length, prepend a zero nibble to make it even.
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
