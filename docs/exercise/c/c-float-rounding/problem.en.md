[ CLEARANCE NOTICE · ONE LEFT ]

The last decimal in the shop hangs on the tag by the door.

Tag price: two decimal places, trailing zeros kept, exactly what the till shows.
Rounded price: the integer part only, the fraction handled by the shop rule, exactly what the owner writes in the book.

The shop rule has a single clause: when the fraction is exactly one half, go to the nearest even integer; otherwise round as usual.

Both prices share one line, one space apart. While stocks last.

**In plain words**: read one `double`, print it with `%.2f` and then with `%.0f`, separated by a single space on one line.

**Input**: one line with a decimal number, possibly negative, with absolute value at most `10^9`.

**Output**: one line with the two-place value and the integer value, separated by a single space.

### Example

Input:

```text
19.99
```

Output:

```text
19.99 20
```

The two decimal places are kept as they are; on the integer scale `19.99` sits closer to `20`, so the rounded price is `20`.

Input:

```text
2.5
```

Output:

```text
2.50 2
```

The fraction is exactly one half, so the value goes to the nearest even integer and `2.5` becomes `2`; `3.5` is also a half and becomes `4`.

### Contract

- `%.2f` and `%.0f` each round by the rules of `printf`, padding with zeros when there are too few decimals.
- Exact halves follow the current rounding mode, that is, the nearest even integer.
- Negative values keep their sign; the rounded price of `-0.6` is `-1`.
- No trailing spaces at the end of the line.
- Print nothing beyond the answer.
