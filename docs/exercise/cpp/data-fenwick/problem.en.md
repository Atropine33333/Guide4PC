Maintain a sequence of length `n`, initially all zeros, with point additions and range-sum queries. Use a Fenwick tree so every operation is `O(log n)`.

### Input

The first line holds two integers `n` and `q`. Then `q` lines, each a command:

- `add i x`: add `x` to position `i` (`x` may be negative).
- `sum i`: print the sum of the first `i` positions.
- `range l r`: print the sum of positions `l` through `r` inclusive, with `l <= r`.

Positions are 1-based.

### Output

One line per `sum` or `range` command, in order. Answers fit in a 64-bit integer.

### Example

```
8 5
add 3 5
sum 3
add 1 2
sum 4
sum 8
```

Output:

```
5
7
7
```

### Constraints

- `1 <= n, q <= 200000`
- `|x| <= 1000000000`
- Every answer fits in a signed 64-bit integer.
