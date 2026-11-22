Read an odd integer `n` and draw a diamond of height `n` with asterisks.

Row `i` (`0 ≤ i < n`) contains `2 * min(i, n - 1 - i) + 1` asterisks, centered inside a field that is `n` characters wide: pad with spaces on the left and nothing on the right.

**Input**: one odd integer `n` (`1 ≤ n ≤ 51`).

**Output**: `n` lines made of spaces and `*`. The widest row starts in column 1 with no leading spaces; every other row is preceded by `(n - stars) / 2` spaces. Do not print extra spaces at the end of a line.

### Example

Input:

```text
5
```

Output:

```text
  *
 ***
*****
 ***
  *
```

### Constraints

- `n` is always odd; even values need no special handling.
- The middle row has exactly `n` asterisks.
- The top and bottom halves are mirror images; either use two nested loops or compute the number of spaces and stars per row first.
- Trailing spaces are ignored when comparing, but leading spaces must be correct.
