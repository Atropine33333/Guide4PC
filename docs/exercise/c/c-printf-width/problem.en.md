Read several rows of names and scores and lay them out in a table with fixed column widths; the header line uses the same widths.

**Input**:

- The first line holds one integer `n` (`1 ≤ n ≤ 50`).
- The next `n` lines each hold a name and an integer score, separated by one space. Names contain lowercase letters only and are between `1` and `8` characters long; scores lie between `-9999` and `9999`.

**Output**:

The first line is the header: `name` left-aligned in `8` columns, `score` right-aligned in `5` columns, with no extra separator — the padding of the name is the gap. Then `n` lines follow in input order with the same widths: name left-aligned in `8` columns, score right-aligned in `5` columns.

### Example

Input:

```text
3
alice 120
bob -42
cara 7
```

Output (each `·` stands for one space, shown so the columns are visible):

```text
name····score
alice·····120
bob·······-42
cara········7
```

In the real output every `·` is an ordinary space.

### Contract

- Produce the header with the same `%-8s` and `%5s` widths; do not type fixed runs of spaces by hand.
- A name exactly `8` characters wide gets no padding, and its score is still right-aligned in `5` columns.
- A score that fills all `5` columns (for example `-9999`) gets no padding in front.
- Print no blank lines beyond the newline that ends each row, and no trailing spaces.
