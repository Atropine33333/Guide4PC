Take the input one line at a time with `fgets`, drop the newline at the end, and put a number in front.

**Input**: between `1` and `20` lines, each at most `200` characters and possibly containing spaces and punctuation; every line ends with a newline.

**Output**: each line as `[<number>] <original line>`, numbering from `1`, with one space between the bracket and the text; an empty line is printed as `[<number>]` with nothing after the bracket.

### Example

Input:

```text
alpha
beta
```

Output:

```text
[1] alpha
[2] beta
```

### Contract

- Use `fgets` in a loop until the end of the input.
- Remove the newline at the end of the line, and a carriage return as well if one is present.
- Keep the leading spaces of the original line.
- No trailing spaces at the end of a line.
