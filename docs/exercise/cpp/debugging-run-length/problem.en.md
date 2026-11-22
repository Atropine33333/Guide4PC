Now hear this: a line of lowercase letters, neatly ranked — now three `a`s, now two `b`s, and a lone `c` at the end.

Ladies and gentlemen, today's tale is called run-length encoding. How does the encoding go? Letters of the same run are gathered into one place: first the letter, then the count of how many times it came in a row. The pieces stand side by side with no punctuation between them. A passage such as `aaabbc` is told as `a3b2c1`.

A certain scribe, telling it thus far, forgot the final run. Consider: at the end of any tale the last sentence matters most, and the last run must be counted out as well. In his program the tally is written only when a letter changes; when the line runs out, he lays down his brush, and the last run vanishes from the record.

One more thing to mind: the count is written in plain decimal without leading zeros; a run holds at most 200 letters, so the count stops at 200, three digits at most.

Enough talk. A line of letters goes in, a line of encoding comes out.

**In plain words**: read one line of lowercase letters and run-length encode it: each maximal run of the same character becomes "character + count", and the pieces are concatenated with no separators. The last run must be printed too, and counts use ordinary decimal without leading zeros. The starter forgets to flush the final run after the loop; add that. Keep the input/output format and change only that loop in `starter.cpp`.

**Input**: one line of lowercase letters, length between 1 and 200.

**Output**: one line with the run-length encoding.

### Example

Input:

```text
aaabbc
```

Output:

```text
a3b2c1
```

### Constraints

- Each run has at most 200 characters; writes counts in ordinary decimal without leading zeros.
- The last run must be printed too; do not drop it.
- Do not change the input/output format; only fix the bug in the starter.
