(The reader shuffles and lays a square spread on the table. The third card is the Empress, leaning back on her throne; the thirteenth is Death with his scythe.)

"Tonight only queens take their seats," she says. "As many queens as there are rows, one square each. No two in the same column, and none sharing a diagonal, or their glances will collide."

She gathers the deck and leaves the empty spread. "Count how many ways the queens can all be seated."

**In plain words**: read `n` and print the number of ways to place `n` queens on an `n × n` board so that no two share a row, a column or a diagonal.

**Input**: one line with an integer `n` (`1 ≤ n ≤ 10`).

**Output**: one line with the number of arrangements, or `0` when there is none.

### Example

Input:

```text
4
```

Output:

```text
2
```

Four queens on a 4 × 4 board admit exactly two non-attacking arrangements.

### Contract

- Every row holds exactly one queen, so an arrangement is determined by its columns.
- Two queens may not share a row, a column, or either diagonal.
- `n` is between `1` and `10`; the largest answer is `724`.
- Print `0` when there is no arrangement.
- No trailing spaces at the end of the line.
