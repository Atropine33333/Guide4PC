Two arrays stand ready: array A has `n` rows and `p` columns, array B has `p` rows and `q` columns, all entries written as integers. What array arises when they are multiplied?

The rule: take row `i` of A and column `j` of B, multiply them term by term, sum the products, and set the total in row `i`, column `j` of the new array. When the columns run out, turn to the next row. The new array has `n` rows and `q` columns, and its entries are `C[i][j] = sum(A[i][k] * B[k][j])` for `0 ≤ k < p`. Within a row, separate entries with one space, and add none at the end.

A certain reckoner confused rows with columns when reading B: B has `p` rows and `q` columns, yet he took the column count for the row count. His results do not match the rule. Correct the two indices.

**In plain words**: read matrix `A` (`n × p`) and matrix `B` (`p × q`) and print the product `C = A * B` (`n × q`), `q` integers per line separated by single spaces. `C[i][j]` is the sum of products of row `i` of `A` and column `j` of `B`. The starter writes the access to `B` as something other than `[k][j]`; fix it without changing the output format.

**Input**:

- The first line holds three integers `n`, `p`, and `q` (`1 <= n, p, q <= 50`).
- The next `n` lines each hold `p` integers, the matrix `A` (`n x p`).
- The next `p` lines each hold `q` integers, the matrix `B` (`p x q`).

**Output**:

- `n` lines each holding `q` integers, the matrix `C = A * B`; adjacent integers on a line are separated by one space.
- `C[i][j] = sum(A[i][k] * B[k][j])` for `0 <= k < p`.

### Example

Input:

```text
2 2 2
1 2
3 4
5 6
7 8
```

Output:

```text
19 22
43 50
```

### Constraints

- Entries lie in `[-100, 100]`; every dot product fits in `int`.
- The starter has an index bug; fix it without changing the input or output format.
- Do not print extra spaces or lines.

### Hint

Check the two indices used to access `B` in the inner loop: `B` has `p` rows and `q` columns.
