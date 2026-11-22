"Oh, good heavens," said the producer, stubbing out his cigarette, "who is supposed to read this report? Nothing but figures, row piled on row."

"Patience, my dear," said the draughtsman, unrolling a sheet. "We shall draw it as horizontal bars. Each line is numbered from 1, then a colon, then one space, then the `#` marks; and where a value is zero we draw a single dash `-`, so the line does not sit empty."

"The numbering starts at 1, not at 0," added the producer. "And no trailing space at the end of a line — the audience has sharp eyes."

"Now that is more like it." He rolled up the sheet. "On my desk by midnight."

**In plain words**: read `n` non-negative integers and draw a horizontal bar chart in input order; each line begins with the index, a colon and one space, followed by as many `#` as the value, with a single `-` standing in for zero.

**Input**:

- The first line holds one integer `n` (`1 ≤ n ≤ 50`).
- The second line holds `n` integers `v` (`0 ≤ v ≤ 40`).

**Output**: `n` lines. Line `i` prints the index (starting at 1), a colon and one space, then `v_i` copies of `#`; when `v_i = 0`, print a single `-` in place of the bar.

### Example

Input:

```text
3
3 0 5
```

Output:

```text
1: ###
2: -
3: #####
```

### Constraints

- No extra trailing space at the end of any line.
- The index starts at 1, not at 0.
