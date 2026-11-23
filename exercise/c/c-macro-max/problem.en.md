Three integers stand in a row. Take the larger of the first two, then compare that result with the third and keep the larger one. The comparison is written once as a macro `MAX(a, b)`, and both comparisons call it.

**Input**: one line with three integers `a`, `b` and `c`, each within the range of `int`.

**Output**: one line with two integers, `MAX(a, b)` and `MAX(MAX(a, b), c)`, separated by a single space.

### Example

Input:

```text
3 5 4
```

Output:

```text
5 5
```

`5` is the larger of the first two; compared with `4`, it is still the larger one.

### Contract

- `MAX` is a function-like macro; in its expansion every parameter is parenthesized, and so is the whole expression.
- The comparison uses a single conditional expression and no semicolon.
- Print only after all three numbers have been read successfully.
- No trailing spaces at the end of the line, and nothing beyond what the statement asks for.
