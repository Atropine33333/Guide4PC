A line of text sits in front of you, and the integer at its head is yours to take. The rules match the standard `atoi`: skip the leading spaces, accept an optional sign, then read digits from left to right and stop at the first character that is not a digit — everything past it is ignored. If no digit is read at all, the answer is `0`.

**In plain words**: convert the integer prefix of the input line and print it, following the same rules as `atoi`.

**Input**: one line of text, length between `1` and `200`, possibly containing spaces, letters and other symbols.

**Output**: one integer — the value picked up by the rules above.

### Example

Input:

```text
   -42abc
```

Output:

```text
-42
```

The two leading spaces are skipped, `-` is the sign, `42` is read, and the letter `a` stops the scan; the trailing `abc` is ignored.

### Contract

- The value fits in `int`, that is, within `-2147483648` to `2147483647`.
- Leading zeros are dropped; `007` and `7` are the same value.
- You may call `atoi` or parse the characters yourself; both give the same result.
- No trailing spaces at the end of the line.
