INSTRUCTION SHEET FOR THE SENTENCE PRESS

Model: SENTENCE-1
Purpose: press a name and an age into a single line of label text.

1. Installation

Connect the press to standard input, fit it with `snprintf`, and keep a buffer large enough to catch the finished sentence. A small buffer truncates the sentence; for this job `80` bytes are enough.

2. Loading

Put the name in the left hopper and the age in the right, with one space between them. The name must not exceed `32` characters and must contain no space; the age is an integer from `0` to `150`.

3. Operation

Press the button and the machine follows one fixed mould:

```text
name=<name>; age=<age>
```

After `name=` the name goes in unchanged, then a semicolon, one space, `age=`, and finally the age in decimal.

4. Output

The product occupies one line and ends with a newline. The space after the semicolon is required, no space may be added around either equals sign, and not one character of the name may change.

5. Notes

- Copy the lowercase letters, the semicolon and the space exactly as the mould shows.
- Assemble the line with `snprintf` in a buffer first, then print the whole line.
- An age of `0` is written as `age=0` like any other.

**In plain words**: read a name without spaces and an age, use `snprintf` to assemble `name=<name>; age=<age>`, and print the line.

**Input**: one line with the name and the age separated by a single space. The name is `1` to `32` characters long and contains no space; the age is an integer from `0` to `150`.

**Output**: one line in the form `name=<name>; age=<age>`, ending with a newline.

### Example

Input:

```text
Alice 30
```

Output:

```text
name=Alice; age=30
```

### Contract

- `name=`, `; age=` are all lowercase, with exactly one space after the semicolon.
- The characters of the name are copied unchanged.
- The age is written as a decimal integer with no leading zeros.
- No trailing spaces at the end of the line.
- Print nothing beyond the answer.
