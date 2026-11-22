One line of text carries letters, digits, spaces and other symbols. Send every character to its own kind and count how many each kind holds.

**In plain words**: read one line of text (possibly empty), count its letters, digits, whitespace characters and everything else, then print the four counts in order.

**Input**: one line of length `0` to `1000`, which may contain letters, digits, spaces, tabs and visible punctuation, without a newline.

**Output**: one line with four integers — letters, digits, whitespace characters, other characters — separated by single spaces.

### Example

Input:

```text
Hello123
```

Output:

```text
5 3 0 0
```

`H`, `e`, `l`, `l`, `o` are letters and `1`, `2`, `3` are digits; there is no whitespace and nothing else.

### Contract

- Letters are `A`–`Z` and `a`–`z`; digits are `0`–`9`.
- Whitespace means the space and the tab.
- An empty line prints `0 0 0 0`.
- No trailing spaces at the end of the line.
- Print nothing beyond the answer.
