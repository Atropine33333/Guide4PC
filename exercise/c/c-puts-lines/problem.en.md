Dubbed film: reading the lines

(The house lights go down; one lamp still burns in the dubbing booth.)

Voice A: Oh, listen, old friend — this reel has to be read one line at a time.

Voice B: One line at a time? Then every line needs a number first.

Voice A: Right. The first is `L1`, the second `L2`; the number takes a colon, and then one space.

Voice B: And the original words go after that, not a syllable missing.

Voice A: What about a blank line with nothing to say?

Voice B: Then we call the number and add nothing after the colon.

(The record button goes down.)

**In plain words**: read a number of lines and print each one in order with the prefix `L<number>: `; a blank line is printed as `L<number>:` alone.

**Input**: between `1` and `20` lines, each at most `200` characters and possibly containing spaces and punctuation, separated by newlines.

**Output**: each line as `L<number>:` followed by one space and the original text, with leading spaces of the original kept. A blank line becomes `L<number>:` with no space after the colon.

### Example

Input:

```text
Call me Ishmael.
The sea was calm.
```

Output:

```text
L1: Call me Ishmael.
L2: The sea was calm.
```

### Contract

- Numbers start at `1` and increase by one per line.
- Strip the newline at the end of the line before adding the prefix.
- Keep every space and punctuation mark of the original line.
- No trailing spaces at the end of a line.
