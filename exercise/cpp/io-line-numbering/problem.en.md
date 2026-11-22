Read every line from standard input until EOF, then print each line with a line number.

### Output format

Let `n` be the total number of lines, including empty lines. Let `w` be the number of decimal digits in `n`. For line `k` (starting at 1):

```text
<line number right-aligned in width w>|<original line>
```

Pad the line number with spaces on the left. Do not put a space after `|`. Reproduce the original line exactly, including leading and trailing spaces. An empty line prints its number and `|`, then ends immediately.

If the input is empty (zero lines), print nothing.

### Example

Input:

```text
alpha

gamma
```

There are 3 lines and the second one is empty, so `w = 1`. Output:

```text
1|alpha
2|
3|gamma
```

### Constraints

- The input may or may not end with a newline; a final segment without a newline still counts as a line.
- Each line has at most 200 characters and there are at most 1000 lines.
- Lines contain only printable ASCII characters and spaces; no tabs or `\r`.
- You cannot print first and fix the numbers later: the total is needed to choose the width.
