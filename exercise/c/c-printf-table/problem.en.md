Read several rows of names and scores and lay them out as a table with fixed columns.

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 50`).
- The next `n` lines each contain a name and an integer score, separated by one space. A name contains only lowercase letters, has length between `1` and `10`, and has no spaces; a score is between `-9999` and `9999`.

**Output**:

The first line is the header `name      score`. Then print `n` rows in input order, using the same column widths as the header:

- the name is left-aligned in at least `8` columns, padded with spaces on the right;
- after the name, leave `2` blank columns;
- the score is right-aligned in `5` columns, padded with spaces on the left.

A name longer than `8` characters is printed in full, without truncation, and the score still follows after two spaces. Do not leave trailing spaces at the end of a line.

### Example

Input:

```text
3
alice 120
bob -42
cara 7
```

Output (the `·` characters below stand for single spaces, to make them visible):

```text
name······score
alice·······120
bob·········-42
cara··········7
```

In the real output, each `·` is an ordinary space.

### Contract

- The header uses the same columns: `name` left-aligned in `8` columns, `2` blank columns, then `score` right-aligned in `5` columns.
- A name that is exactly `8` characters wide still has `2` spaces after it before the score.
- A score that exactly fills `5` columns gets no extra padding.
- Use `printf` field widths to align the columns; do not build the spaces by hand.
- Do not print blank lines apart from the newline that ends each line.
