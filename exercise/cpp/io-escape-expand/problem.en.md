This exercise reads no stdin and writes no stdout. Implement a function that expands escape sequences in text; a hidden harness runs the tests.

```cpp
std::string expand_escapes(const std::string& input);
```

Rules:

- `\n` becomes a newline character (`'\n'`).
- `\t` becomes a tab (`'\t'`).
- `\r` becomes a carriage return (`'\r'`).
- `\\` becomes one backslash.
- When a backslash is followed by any other character, keep both the backslash and that character unchanged.
- A lone backslash at the end of the input is kept as a backslash.
- Every other character is copied as-is, including spaces, digits, and punctuation.
- Do not write `main`.

### Hint

Scan from left to right. When you see a backslash, first check whether another character follows: if not, emit the backslash; if so, decide what to emit from the next character and skip both. For an unknown escape, emit the backslash and the following character.
