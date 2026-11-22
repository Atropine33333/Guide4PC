One line carries the two assignments `x=...` and `y=...`, with commas and spaces in no fixed places. Read the whole line into a string first, then pull the two integers out of it with `sscanf`.

**Input**: one line shaped like `x=<integer>,y=<integer>`. Spaces are allowed between the letters `x`, `y` and the `=`, between `=` and the digits, and on both sides of the comma — or absent entirely. The comma itself is required, and `x` comes before `y`.

**Output**: one line with a single integer, the value of `x + y`.

### Example

Input:

```text
x = -4 , y = 9
```

Output:

```text
5
```

### Contract

- Read the line into a string with `fgets` first, then parse it with `sscanf`.
- The number of spaces varies; the order of the letters, signs and comma is fixed.
- Both values fit in `int`, and so does the sum.
- No trailing spaces.
