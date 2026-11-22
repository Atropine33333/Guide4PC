Read `n` non-negative integers and draw a horizontal bar chart in input order.

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 50`).
- The second line contains `n` integers `v` (`0 ≤ v ≤ 40`).

**Output**: `n` lines. Line `i` starts with the 1-based index, a colon, and one space, followed by `v_i` copies of `#`. If `v_i = 0`, print a single `-` instead of a bar.

### Example

Input:

```text
3
3 0 5
```

Output:

```text
1: ###
2: -
3: #####
```

### Constraints

- Do not print trailing spaces.
- Indexes start at 1, not 0.
