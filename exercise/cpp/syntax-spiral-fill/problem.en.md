Read an integer `n` (`1 ≤ n ≤ 20`) and output an `n × n` spiral matrix: start at the top-left corner, write `1`, then `2, 3, …`, moving right and turning clockwise whenever the next cell would leave the grid or is already filled, until all `n²` cells are used.

Format: `n` integers per line, exactly one space between neighbours, each integer right-aligned to width `W`, where `W` is the number of decimal digits of `n × n` (`W = 2` for `n = 4`, `W = 3` for `n = 10`). No trailing spaces.

### Input

A single integer `n`.

### Output

`n` lines, each with `n` right-aligned integers.

### Example

Input:

```
5
```

Output:

```
 1  2  3  4  5
16 17 18 19  6
15 24 25 20  7
14 23 22 21  8
13 12 11 10  9
```

### Constraints

- `1 ≤ n ≤ 20`; the input is always valid.
- Standard library only; print no extra text.
