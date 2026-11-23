# Turn It Over

**In plain words**: read an `n` by `m` matrix of integers and print its transpose: `m` rows and `n` columns, where the value at row `i`, column `j` of the original appears at row `j`, column `i` of the result.

**Input**: the first line holds two integers `n` and `m` (`1 ≤ n, m ≤ 50`). The next `n` lines each hold `m` integers, none larger than `1000` in absolute value.

**Output**: `m` lines, each with `n` integers separated by single spaces, with no trailing space at the end of a line.

### Example

Input:

```text
2 3
1 2 3
4 5 6
```

Output:

```text
1 4
2 5
3 6
```

The three columns that lay flat are now three rows standing up.

### Contract

- Rows and columns swap: the result is `m` rows by `n` columns.
- Do not print anything beyond the required numbers.
- Do not leave extra spaces at the end of a line.
