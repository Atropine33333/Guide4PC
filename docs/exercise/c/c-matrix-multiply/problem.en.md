When two matrices are multiplied, the entry in row `i` and column `j` of the result is the dot product of row `i` of the left factor and column `j` of the right factor: multiply term by term, then add. The left factor's row count gives the result's rows; the right factor's column count gives the result's columns.

**Input**:

- The first line holds three integers `n`, `m`, `k` (`1 ≤ n, m, k ≤ 50`): the left matrix `A` has `n` rows and `m` columns, the right matrix `B` has `m` rows and `k` columns.
- The next `n` lines hold `m` integers each — `A`, row by row.
- The next `m` lines hold `k` integers each — `B`, row by row.
- Every entry lies between `-100` and `100`.

**Output**: `n` lines, each with `k` integers — the product `C = A × B`; entries on a line are separated by single spaces.

### Example

Input:

```text
2 3 2
1 2 3
4 5 6
7 8
9 10
11 12
```

Output:

```text
58 64
139 154
```

The first entry of `C` is `1*7 + 2*9 + 3*11 = 58`; the others follow the same rule.

### Contract

- `C[i][j] = A[i][0]*B[0][j] + A[i][1]*B[1][j] + … + A[i][m-1]*B[m-1][j]`.
- Each entry of the result has absolute value at most `50 × 100 × 100 = 500000`, which fits in `int`.
- No trailing spaces at the end of a line.
