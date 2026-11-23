Two passages laid side by side always share some characters that appear in the same order in both. Stretch that shared part as long as it will go — how long is it?

**Input**: two lines, each a string without spaces, containing only lowercase English letters, each between `1` and `2000` characters long.

**Output**: one line with a single integer — the length of the longest common subsequence of the two strings.

### Example

Input:

```text
abcbdab
bdcaba
```

Output:

```text
4
```

`bcba` is a subsequence of both strings, and no common subsequence of length `5` exists.

### Contract

- A subsequence may skip characters but must keep their relative order; it need not be contiguous.
- Both strings are non-empty and contain only lowercase letters.
- Print the length only; the subsequence itself is not required.
- The table is `2000 × 2000`; two rolling rows suffice, so a full 2-D table is unnecessary.
- No trailing spaces at the end of the line.
