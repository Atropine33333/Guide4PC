Read one line of characters and print the length of the longest run of identical characters.

**Input**:

- One line of text, which may contain spaces, with length between `0` and `1000`.
- The trailing newline is not part of the text.

**Output**:

- One integer: the length of the longest run.
- An empty line prints `0`.

### Example

Input:

```text
aabbbbcc
```

Output:

```text
4
```

### Constraints

- Characters are case-sensitive; spaces are ordinary characters.
- The starter contains a buggy implementation; fix it without changing the input or output format.
- Only the calculation needs changing; do not rewrite the I/O.

### Hint

The starter updates the answer only when the character changes, so it misses a run that ends at the end of the line. Think about where else the answer must be updated.
