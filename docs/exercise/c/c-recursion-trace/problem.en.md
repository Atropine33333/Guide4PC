Factorial written recursively is a trail of footprints: to compute `n!`, take one step to `(n-1)!`, then one more ... until you reach `0!`, then walk back along the same prints, multiplying at every step.

Count the footprints: every entry into `fact` bumps a counter, including the final stop at `0`.

**In plain words**: read `n`, compute `n!` recursively, and count how many times the recursive function was entered; print the factorial and the call count. `fact(0)` is entered once, so `n = 0` prints `1 1`.

**Input**: one line with an integer `n` such that `0 ≤ n ≤ 12`.

**Output**: one line with two integers — the factorial, then the number of calls — separated by a single space.

### Example

Input:

```text
3
```

Output:

```text
6 4
```

`fact(3)` calls `fact(2)`, which calls `fact(1)`, which calls `fact(0)`: four entries in all.

### Contract

- `fact` bottoms out at `n == 0` and returns `1`.
- `12! = 479001600` fits in an `int`; a `long long` is safer still.
- The call count is the number of nodes in the recursion tree: `n + 1` for every `n ≥ 0`.
- No trailing spaces at the end of the line.
