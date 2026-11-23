`strlen` finishes counting a line and hands back a `size_t`. That type is made for lengths: unsigned, wide enough, and printed with `%zu`.

**In plain words**: read one whole line (spaces allowed inside), drop the trailing newline, and print the number of characters together with that number converted to `int`, separated by one space.

**Input**: one line of length between `0` and `1000`, containing visible ASCII characters and spaces, ending with a newline.

**Output**: one line with two integers — the character count and the value after the `(int)` conversion — separated by one space.

### Example

Input:

```text
a b  c
```

Output:

```text
6 6
```

The line is `a`, space, `b`, space, space, `c` — `6` characters in all.

### Contract

- The final newline is not counted; spaces at the end of the line are.
- Store the result of `strlen` in a `size_t` and print it with `%zu`.
- Print the converted value as an `int` with `%d`; the length never exceeds `1000`, so the conversion does not overflow.
- No trailing spaces at the end of the line.
