Read several "name score" lines and print them as two aligned columns.

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
