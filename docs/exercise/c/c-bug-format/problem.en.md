The program in `starter.c` should print a table of squares, but every row is crammed onto the same line and the columns do not line up.

The table has `n` rows. Row `i` prints `i`, then `i * i`. Both numbers take a fixed field width and are right-aligned: `i` takes `4` columns and `i * i` takes `6`, the fields sit next to each other, and each row ends with a newline.

Fix the output format.

**In plain words**: read `n` and print `n` rows; row `i` holds `i` and `i * i` in right-aligned fields of `4` and `6` columns, one row per line.

**Input**: one line with a single integer `n` (`1 ≤ n ≤ 100`).

**Output**: `n` rows. Row `i` (for `i` from `1` to `n`) holds `i` and `i * i`, in fields of `4` and `6` columns, both right-aligned, with no extra space between the fields.

### Example

Input:

```text
3
```

Output:

```text
   1     1
   2     4
   3     9
```

In the first row, `1` has three spaces before it; five spaces follow it before the square `1`.

### Contract

- Use `printf` field widths for the alignment; do not build the padding by hand.
- End every row with a newline, including the last one.
- Change only the output spot; keep the reading part as it is.
