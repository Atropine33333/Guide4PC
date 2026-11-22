A macro only moves text around before compilation; where the parentheses sit decides what binds to what after the move. With `SQ(v)` defined as `((v) * (v))`, `SQ(x + 1)` expands to `((x + 1) * (x + 1))` and `SQ((x) + 1)` to `(((x) + 1) * ((x) + 1))`; the two agree. The function `square` first turns its argument into a value and then squares it, which also yields `(x + 1) * (x + 1)`.

**In plain words**: read `x`, then print the values of `SQ(x + 1)`, `SQ((x) + 1)`, and `square(x + 1)` as three integers on one line.

**Input**: one line with a single integer `x` (`-100 ≤ x ≤ 100`).

**Output**: one line with three integers separated by single spaces.

### Example

Input:

```text
2
```

Output:

```text
9 9 9
```

### Contract

- `#define SQ(v) ((v) * (v))`; `square` is an ordinary function with the signature `int square(int v)`.
- The parentheses inside the macro body keep `x + 1` together; if the body were `v * v`, then `SQ(x + 1)` would expand to `x + 1 * x + 1`.
- The macro evaluates its argument twice while the function evaluates it once; this argument has no side effects, so both agree.
- No trailing spaces at the end of the line.
