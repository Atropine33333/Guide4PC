Read two matrices and print their product.

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
