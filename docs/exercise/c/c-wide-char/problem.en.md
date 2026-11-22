Dubbed film: the wide-char waiting room

(The projector clicks; one line of subtitles appears on the screen.)

Voice A: Oh, listen, old friend — every character in this line is about to be called by name.
Voice B: Called by name? They are just a string of ASCII characters.
Voice A: In the wide-character world each one carries its own number, and not a single one may be left out.
Voice B: Then we read them in order: `H` is `72`, `i` is `105`.
Voice A: One number, one space, then the next.
Voice B: And if the line is empty?
Voice A: Then nothing is called, and the screen keeps a single blank line.

(Music up; the subtitles scroll slowly.)

**In plain words**: read one line of ASCII characters, treat each character as a `wchar_t`, and print its decimal value, with one space between values.

**Input**: one line of length between `0` and `1000`, containing only printable ASCII characters (`0x20` to `0x7E`), ending with a newline.

**Output**: one line listing the `wchar_t` decimal value of each character in order, separated by single spaces; an empty line produces just a newline.

### Example

Input:

```text
Hi
```

Output:

```text
72 105
```

`H` has code `72`, and `i` has code `105`.

### Contract

- For characters in the ASCII range, the `wchar_t` value equals the character code, that is, the familiar ASCII value.
- The final newline is not called; spaces in the middle and at the end are, with value `32`.
- No trailing spaces at the end of the line.
