A matrix with `m` rows and `n` columns is mostly `0`. Only the nonzero positions are recorded: each nonzero element becomes a triplet line `row column value`, with rows and columns numbered from `0`. After transposing, the entry at row `r`, column `c` moves to row `c`, column `r` of the new matrix, with its value unchanged.

**Input**: the first line holds three integers `m`, `n` and `k` — the number of rows, the number of columns, and the number of nonzero elements. Then come `k` lines, each with three integers `r c v`, meaning row `r`, column `c` holds `v`.

**Output**: the first line holds three integers `n`, `m` and `k` — the rows, columns and nonzero count after transposing. Then come `k` lines, each with three integers `c r v`: the new row, the new column and the value. All triplets are ordered by the new row, and within one new row by the new column, both ascending.

### Example

Input:

```text
3 4 4
0 1 5
1 3 -2
2 0 7
2 2 9
```

Row `0`, column `1` holds `5`, which lands at row `1`, column `0`; row `1`, column `3` holds `-2`, which lands at row `3`, column `1`; `2 0 7` and `2 2 9` land at `0 2` and `2 2`.

Output:

```text
4 3 4
0 2 7
1 0 5
2 2 9
3 1 -2
```

### Contract

- `1 ≤ m, n ≤ 500` and `0 ≤ k ≤ 1000`.
- The input triplets are not necessarily ordered; each `(r, c)` appears at most once.
- `v` is never `0`, and `|v| ≤ 10^9`.
- No trailing spaces at the end of a line, and nothing beyond the requested output.
