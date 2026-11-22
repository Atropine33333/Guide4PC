Count the lines, the words, and the characters other than newline in a piece of text.

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 50`).
- The next `n` lines contain lowercase letters, digits, and spaces. Lines may be empty and may start or end with spaces.

**Output**: one line with three integers — line count, word count, and character count excluding newlines — separated by single spaces.

A word is a maximal run of non-space characters. An empty line counts as a line but contributes no word. Spaces at the start, at the end, and between words all count as characters.

### Example

Input:

```text
2
hello world
foo
```

Output:

```text
2 3 14
```

### Hint

`std::cin >> word` skips all whitespace and cannot distinguish blank lines or count spaces. Use `std::getline` per line and count characters yourself.
