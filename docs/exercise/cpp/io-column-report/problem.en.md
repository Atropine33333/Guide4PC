A: Tell me, how does one set out this ledger of names and scores?

B: Simple enough. The name goes on the left, twelve characters wide, padded with spaces on the right; right after it a bar `|`, with not one space on either side.

A: And the score?

B: On the right, eight characters wide, padded on the left. A name that fills all twelve and a score that fills all eight get nothing added. And no trailing spaces at the end of any line.

A: What if a name runs past twelve?

B: Names here are ten at most, so spare yourself the worry. Scores can reach `-999999`, seven characters, still inside eight.

A: The bar itself tolerates no spaces?

B: None. The padding is the whole separation; add one by hand and the line ends with a stray space.

**In plain words**: read `n` lines of "name score" and print each as: the name left-aligned in 12 characters, one `|`, the score right-aligned in 8 characters, with no trailing spaces.

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 100`).
- The next `n` lines each contain a name and an integer score separated by one space. Names contain only lowercase letters and are 1 to 10 characters long; scores are between `-999999` and `999999`.

**Output**: `n` lines in input order. Each line is formatted as:

- the name, left-aligned in a field of width 12, padded with spaces;
- immediately followed by a vertical bar `|`;
- the score, right-aligned in a field of width 8, padded on the left.

### Example

Input:

```text
2
alice 120
bob -42
```

Output (dots show spaces for readability):

```text
alice·······|·····120
bob·········|·····-42
```

In the real output the dots are ordinary spaces.

### Constraints

- Do not add extra spaces around the name, the bar, or the score; the padding is the entire separation.
- When a name or score exactly fills its column, add no padding.
- There is exactly one bar, with no fixed spaces around it.
- Do not leave trailing spaces at the end of a line.
