Read from the first character to the last and count characters, lines and words in one pass.

**Input**: one or more lines, at least one, with a total length of at most `10000`; printable ASCII characters, spaces and newlines only, and the last line ends with a newline.

**Output**: one line with three integers — character count, line count, word count — separated by single spaces.

- Character count: every character, including spaces and newlines.
- Line count: the number of newline characters.
- Word count: a run of non-whitespace characters is one word; punctuation attached to a word counts as part of it.

### Example

Input:

```text
the quick fox
```

Output:

```text
14 1 3
```

The line has `13` visible characters; with the newline at the end that makes `14` characters, `1` line and `3` words.

### Contract

- Read with a `getchar` loop until `EOF`.
- Whitespace means space, tab and newline.
- Count lines by newline characters; the final input line carries a newline.
- No trailing spaces.
