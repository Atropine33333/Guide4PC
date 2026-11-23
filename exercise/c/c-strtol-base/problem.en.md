THE KEY OF BASES

The reader turns a single card; a token is carved on it. Sometimes it is a run of digits; sometimes two letters, `0x`, sit in front.

"Two doors," she says. "One opens on ten, one on sixteen. Read the two letters at the front, and the key chooses itself."

Your card reads `0x1F`. Speak the decimal number behind the door.

**In plain words**: read one space-free integer token (hexadecimal with a `0x` prefix, or plain decimal), convert it with `strtol(s, NULL, 0)`, and print the value in base ten.

**Input**: one line with a single token, length between `1` and `30`, containing only digits, letters, an optional `+`/`-`, and a `0x`/`0X` prefix; no spaces.

**Output**: one decimal integer.

### Example

Input:

```text
0x1F
```

Output:

```text
31
```

`0x` marks it as hexadecimal, and `1F` is `1 * 16 + 15 = 31`.

### Contract

- The prefix decides the base: `0x`/`0X` means hexadecimal, anything else decimal.
- Hexadecimal letters are accepted in either case; `0xAbCd` and `0xABCD` are the same.
- The value fits in `int`, so it cannot overflow.
- No trailing spaces at the end of the line.
