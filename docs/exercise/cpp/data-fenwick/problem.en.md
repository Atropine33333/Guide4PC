There is a sequence of `n` places, at first holding nothing, every place zero. Each day an officer brings two kinds of business, interleaved. The first is "add": lay `x` more upon place `i`, and a loss is written as a negative number. The second is "ask": how much stands from the first place through place `i`? There is also the question of a stretch: how much stands from place `l` through place `r`? The askers and the adders come by the ten-thousand.

Thus the method: set up one tablet of `n` marks, called the Fenwick tree. For "add", begin at place `i` and each time add its lowest set bit (`i += i & -i`), recording the number on every mark passed. For the sum from the first place through `i`, begin at `i` and each time strip its lowest set bit (`i -= i & -i`), adding the entries of the marks passed. For `l` through `r`, take the sum through `r` and subtract the sum through `l - 1`. Neither an add nor a question takes more than `O(log n)` steps.

**In plain words**: maintain a sequence of length `n`, initially all zeros, with point additions (`x` may be negative) and prefix or range sum queries; every answer fits in a 64-bit integer. Read `n` and `q`, run the `q` commands in order, and print one line for each `sum` and `range`. Use a Fenwick tree so each operation is `O(log n)`.

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
