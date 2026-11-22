THE LAW OF SET PREFIXES

Article 1 (Subject). Two lines are given: the first is the text, the second is a set of characters. The question is how many characters, counting from the start of the text, lie in that set without a break.

Article 2 (The set). The set consists of the characters on the second line; a character written twice counts as one member. An empty second line means an empty set.

Article 3 (The prefix). Walk from the first character of the text to the right and stop at the first character that is not in the set. The number of characters counted before stopping is the answer.

Article 4 (Empty text). When the text is empty, the answer is zero.

Article 5 (Range). Neither line contains a newline; both have length between `0` and `1000`, and their characters come from visible ASCII plus the space.

Article 6 (Output). The answer occupies one line as a decimal integer, with nothing added.

**In plain words**: read two lines, the text first and the character set second. Print how many characters at the very start of the text belong to the set; print `0` when the text is empty or its first character is outside the set.

**Input**: two lines, the text and the character set, each of length `0` to `1000` and without a newline.

**Output**: one line with a single integer, the length of the matching prefix.

### Example

Input:

```text
hello world
helo
```

Output:

```text
5
```

The five characters `h`, `e`, `l`, `l`, `o` all belong to the set; the sixth character is a space, which does not, so the walk stops at `5`.

### Contract

- Count only the unbroken prefix; once it breaks, stop and ignore the rest.
- Case matters; a space is an ordinary character.
- No trailing spaces at the end of the line.
- Print nothing beyond the answer.
