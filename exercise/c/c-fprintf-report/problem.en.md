Write the identifier and the score into an already-open file in a fixed format.

**Interface**:

```c
void report(FILE *f, int id, int score);
```

- Write one line into `f`: `id=` followed by the decimal `id`, one space, `score=` followed by the decimal `score`, and a newline at the end.
- No leading zeros, no padding, no thousands separators; a negative number carries its minus sign, and zero is written as `0`.
- Each call writes that one line and nothing else; do not close `f` and do not rewind it.
- This function is called directly, so there is no `main` to write.

### Example

After `report(f, 7, 93)`, `f` holds one more line:

```text
id=7 score=93
```

A following `report(f, -1, -5)` appends:

```text
id=-1 score=-5
```

### Contract

- Exactly one newline ends the line.
- Successive calls append their lines in order.
- Both values fit in `int` and may be negative.
- Write with `fprintf`.
