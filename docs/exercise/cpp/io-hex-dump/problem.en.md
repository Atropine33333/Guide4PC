Read every byte from standard input (until EOF) and print a classic hex dump with 16 bytes per line.

### Input

Any number of text lines. The judge always appends one trailing newline, and that byte is part of the dump.

### Output

One line per 16 bytes:

```
OOOOOOOO  XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX |................|
```

- `OOOOOOOO` is the index of the first byte on the line, eight lowercase hex digits, starting at `00000000` and increasing by 16 per line.
- The offset is followed by two spaces.
- The hex area has 16 slots: present bytes print as two lowercase hex digits plus one space; when fewer than 16 bytes remain, every missing byte contributes three spaces.
- Then `|`, then the ASCII form of each byte (printable characters unchanged, everything else as `.`), then another `|`.
- No trailing spaces.

### Example

The input is 16 bytes (15 printable characters plus a newline):

```
0123456789abcde
```

Output:

```
00000000  30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 0a |0123456789abcde.|
```

### Constraints

- Empty input (in practice one newline) still produces one line.
- Hex letters are lowercase.
