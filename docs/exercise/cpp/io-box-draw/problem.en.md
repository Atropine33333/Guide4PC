**Documents Office Circular No. 7 of 2026**

**Notice on the Uniform Specification for Drawing Text Frames**

To all sections and directly subordinate units:

For the orderly presentation of incoming papers, the following provisions on drawing frames are hereby issued.

1. A submission first reports the number of lines `n` (`1 ≤ n ≤ 50`); the `n` lines below it are the body. Each may be empty, may contain spaces, and is at most 40 characters long.

2. The frame has `n + 2` lines in all. The first and last are made of `+`, several `-`, and `+`, the number of `-` being "length of the longest line + 2".

3. Every middle line is written as `| `, the text, padding spaces, and ` |`. Text is left-aligned and padded on the right so that every body row has the width of the longest line.

4. If all body lines are empty, the longest length is 0, the frame is `+--+`, and every content row is `|  |`.

5. A newline still follows the first `n`; consume it before reading the body lines.

Please carry out the specification above accordingly.

cc: Records Room.

**In plain words**: read `n` lines of text and place them inside a left-aligned rectangular frame, printing `n + 2` lines in the format above.

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
