A phone book sorted in ascending order: if the number you want is there, the book opens at its page; if it is not, the book answers `-1`. The book is thick, so instead of scanning page by page you open it in the middle each time: a smaller number sends you into the left half, a larger one into the right half. The range halves with every look, and a few looks settle it.

**Input**:

- The first line holds one integer `n` (`1 ≤ n ≤ 200000`).
- The second line holds `n` strictly increasing integers separated by single spaces, each between `-10^9` and `10^9`.
- The third line holds one integer `key`, also between `-10^9` and `10^9`.

**Output**: one integer — the zero-based index of `key` in the sequence, or `-1` if `key` is not there.

### Example

Input:

```text
7
1 3 5 7 9 11 13
9
```

Output:

```text
4
```

`9` is the fifth number, so its index is `4`.

### Contract

- The sequence is strictly increasing and has no duplicates, so the index is unique.
- Indices start at `0`.
- When the key is absent, print only `-1`.
- With up to 200000 numbers, halving keeps the comparison count near twenty.
- No trailing spaces at the end of the line.
