Given a completion percentage and a progress-bar width, draw a text progress bar.

The bar always spans exactly `width` cells. The number of filled cells uses integer arithmetic:

```text
filled = percent * width / 100      (rounded down)
```

The first `filled` cells are `#`, the rest are `-`, the bar is enclosed in square brackets, and one space plus the percentage and `%` follows it.

**Input**:

- One line with two integers `percent` and `width`, separated by one space.
- `0 <= percent <= 100`, `1 <= width <= 50`.

**Output**:

- One line such as `[###-------] 30%`, containing exactly `width` cells and one percent sign.
- Do not print trailing spaces.

### Example

Input:

```text
30 10
```

Output:

```text
[###-------] 30%
```

### Constraints

- Round down, not to nearest: `99 10` fills only `9` cells.
- With `width = 1`, only `percent = 100` shows `#`.
- Print the percentage exactly as given, without leading zeros.
