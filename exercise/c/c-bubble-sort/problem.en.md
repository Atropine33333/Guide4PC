A row of integers stands out of order. Bubble sort keeps one rule: walk from left to right comparing neighbours, and when the left one is larger, swap them. One pass drops the largest into the last slot; the next pass settles the second largest; after enough passes the whole row is ascending.

**Input**:

- The first line holds one integer `n` (`1 ≤ n ≤ 1000`).
- The second line holds `n` integers separated by single spaces, each between `-10^9` and `10^9`.

**Output**: one line with the `n` integers in ascending order, separated by single spaces.

### Example

Input:

```text
5
5 1 4 2 8
```

Output:

```text
1 2 4 5 8
```

### Contract

- Equal values are all kept; each occurrence is printed.
- No trailing spaces at the end of the line.
- `n` is at least `1`, so the input is never empty.
