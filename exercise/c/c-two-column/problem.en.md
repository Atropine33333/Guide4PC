(A table, two chairs, and a ledger lying open.)

A: Why does your ledger run in two columns?

B: One column for Zhang, one for Li; whoever walks in fills his own column.

A: And after that?

B: A third column after them, adding the two together. Three columns to a row, tight and even, not one digit out of place.

A: What if a row carries only one number?

B: Then the entry is incomplete: cross out the whole row and mark it void.

A: And an empty line?

B: An empty line is a breath; it is no entry, and it is no void either.

A: And when the ledger ends?

B: Close the book and call out: how many real entries, what they add up to, how many rows were crossed out.

**In plain words**: read line by line; take the first two integers `a` and `b` from each line and print `a b sum` with single spaces; once the input ends, print one summary line `total <rows> sum <total> skipped <void rows>`.

**Input**: several lines, each at most 200 characters, at most 1000 lines; every integer satisfies `|x| ≤ 1000000000`. The input may be empty.

**Output**:

- One line per valid row: `a`, `b`, and `a + b`, separated by single spaces.
- One final summary line, shaped like `total 2 sum 4 skipped 0`.

### Example

Input:

```text
1 2
-3 4
```

Output:

```text
1 2 3
-3 4 1
total 2 sum 4 skipped 0
```

Both rows are complete, so the void count is `0`; the row sums are `3` and `1`, and their total is `4`.

### Contract

- The first two integers on a line are the entry; any further integers on that line are ignored.
- A line holding only spaces or tabs is skipped: it counts as neither a valid row nor a void row.
- A line with fewer than two integers is skipped whole and counted in `skipped`.
- Keep the total in a 64-bit signed integer; it may leave the `int` range.
- No trailing spaces at the end of a line, and nothing outside the requested output.
