Read three lines: a text, a pattern, and a replacement. Replace every non-overlapping occurrence of the pattern in the text.

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
