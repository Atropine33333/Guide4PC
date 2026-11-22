A line of text can hide characters you cannot see: a tab, a carriage return, a backslash, a double quote. Name them one by one so they become visible.

**Input**: one line of at most `1000` characters, spaces included; the trailing newline is not content.

**Output**: one line of names in the original character order, separated by single spaces.

- A newline is written `\n`, a tab `\t`, a carriage return `\r`.
- A backslash is written `\\`, a double quote is written `\"`.
- Every other character is copied as it is.
- An empty line has nothing to name; print one empty line.

### Example

Input:

```text
a"b
```

Output:

```text
a \" b
```

### Contract

- A space is an ordinary character and is copied as it is; it has a separating space on each side, so the result shows a run of spaces.
- Read the whole line, spaces included, and drop the trailing newline.
- No trailing spaces at the end of the line.
- Print nothing beyond the answer.
