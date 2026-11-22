Read several lines of text and put them inside a left-aligned rectangular frame.

**Input**:

- The first line holds an integer `n` (`1 ≤ n ≤ 50`), the number of text lines.
- The next `n` lines each hold a piece of text, possibly empty and possibly containing spaces, at most 40 characters long.

**Output**: the finished frame, `n + 2` lines in total:

- The first and last lines are `+`, several `-`, and `+`, where the number of `-` is "length of the longest line + 2".
- Every middle line is `| `, the text, enough padding spaces, and ` |`, so that all text rows have the width of the longest line.

### Example

Input:

```text
3
cat
hi
dog
```

Output:

```text
+-----+
| cat |
| hi  |
| dog |
+-----+
```

### Constraints

- All text lines are left-aligned and padded with spaces on the right.
- If every line is empty, the longest length is 0, the border is `+--+`, and every content row is `|  |`.
- A newline follows the first integer `n`; consume it before reading the text lines.
