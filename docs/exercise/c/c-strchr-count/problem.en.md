"CALLING THE CHARACTER"

How many times does one character appear in a line of text? Read the line, read the character, and walk from the start of the line to the end, making one tally each time the character answers to its name.

**In plain words**: read one line of text and one character, count how many times that character occurs in the line, and print the count.

**Input**:

- The first line is a stretch of text, length between `1` and `200`, spaces allowed, with no line break inside.
- The second line is one visible character that is not a space, tab or newline.

**Output**: one integer — the number of occurrences of that character in the text line.

### Example

Input:

```text
hello world
l
```

Output:

```text
3
```

The letter `l` appears three times in `hello world`.

### Contract

- Use `strchr` from the current position; after a hit, move the pointer past it and search again.
- Matching is case-sensitive; `a` and `A` are different characters.
- Spaces are part of the text and are kept; the character being counted is not whitespace.
- No trailing spaces at the end of the line.
