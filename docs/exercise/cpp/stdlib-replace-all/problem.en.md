(Two in the morning, channel seven. A saxophone plays.)

"Hi there, friends. I'm Ron. Look at that article in your hands: `one two one`. You want every `one` replaced with `1`, but you only have two hands."

"But wait — look at this!"

(Ding!)

"The Miracle Replace-All 3000! Left to right, find a match, replace it, then carry on after the replacement. One `one` becomes `1`; the next `one` becomes `1`. Replacing `a` with `aa` in `aaa`? Six `a`s, with no infinite expansion!"

"Spaces in the text? It takes them all. The pattern is non-empty; the replacement may be empty, and an empty replacement simply erases. Read the whole line — don't let `cin >>` bite off only the first word."

"Not sold in stores. Call now for just $19.99 — but wait, there's more! For the first three input lines, we won't even charge you for the newlines!"

"Operators are standing by! Call 676 - 6767 - 6767 today!"

**In plain words**: Read three lines: the text, the pattern, and the replacement. Scan the text from left to right; when a non-overlapping match is found, replace it and continue scanning after the replacement, never re-matching inside the text just produced. Print one line with the resulting text (possibly empty). The text may contain spaces, so read whole lines; the pattern is non-empty and the replacement may be empty.

**Input**:

- The first line is the text (possibly empty, may contain spaces, at most 1000 characters).
- The second line is the pattern (non-empty, at most 20 characters).
- The third line is the replacement (possibly empty, at most 20 characters).

**Output**: one line with the resulting text (possibly empty).

### Example

Input:

```text
one two one
one
1
```

Output:

```text
1 two 1
```

### Constraints

- Scan from left to right: replace one match, then continue scanning after the replacement; never re-match inside text you just produced.
- For example, text `aaa`, pattern `a`, replacement `aa` produces six `a` characters, not an infinite expansion.
- The text may contain spaces, so `cin >>` cannot read the whole line.
