The Blank-Line Rulebook

1. Every blank line counts.
2. A line holding only spaces or tabs counts as blank too.
3. Blank lines that follow one another settle into one.
4. Blank lines at the start leave nothing behind.
5. Blank lines at the end leave nothing behind.
6. The one that stays holds no character at all.

**In plain words**: read a whole text line by line until the input ends; squash runs of blank lines into one, drop the blank lines at both ends, and print every other line unchanged.

**Input**: several lines of text, each at most 1000 characters, at most 1000 lines in total. The input may be empty.

**Output**: the processed text; the remaining lines come back unchanged, and a blank line is printed as a truly empty line.

### Example

Input:

```text
alpha


beta
```

Output:

```text
alpha

beta
```

Two consecutive blank lines become one; blank lines at either end are dropped as well.

### Contract

- Other characters on a line stay as they are.
- A line of spaces or tabs is treated as blank and printed as a truly empty line.
- When the whole input is blank, the output is empty.
- Do not add spaces at the end of a line.
