Now then, one day an old ledger-keeper came in with a bundle of papers covered in bytes that nobody could read. He sat down, cupped his hands, and said: ladies and gentlemen, do me the kindness of copying this ledger, sixteen to a line.

How is it copied? Sixteen bytes to a line. First the number of the line's first byte, eight lowercase hex digits, starting at `00000000` and rising by sixteen each line. After the number, two spaces.

Then sixteen slots. Where a byte sits in a slot, two lowercase hex digits and one space after; where the line falls short of sixteen, every missing slot gives three spaces, so the columns come out even.

When the slots are done, a bar `|`, then the face of each byte: printable ones written as they are, the rest as a dot `.`, and a closing bar. No trailing spaces, not one.

Someone in the hall asks: what of empty input? Empty input still arrives with a newline, and a line is copied all the same.

**In plain words**: read stdin to EOF and print a classic hex dump, 16 bytes per line: an 8-digit lowercase offset, two spaces, sixteen hex slots, and `|ASCII|`, with missing slots padded by spaces.

### Input

Any number of text lines. The input always ends with one trailing newline, and that byte is part of the dump.

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
