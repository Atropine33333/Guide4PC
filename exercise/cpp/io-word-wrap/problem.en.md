Read a line width and a line of words, then greedily wrap the text to that width.

**Input**:

- The first line contains an integer `W` (`1 ≤ W ≤ 40`).
- The second line contains lowercase words separated by exactly one space, with no leading or trailing spaces. Its length is at most 200.

**Output**: the wrapped text. Each line holds as many complete words as fit, separated by one space, with no leading or trailing spaces. A word longer than `W` must occupy a line by itself and must not be split.

### Example

Input:

```text
10
the quick brown fox jumps
```

Output:

```text
the quick
brown fox
jumps
```

### Constraints

- Keep the input word order; do not reorder words.
- When testing whether the next word fits, include the separating space.
- After reading `W`, the newline is still in the stream; consume it with a `std::getline` or `ignore`.
- The text is non-empty and contains at least one word.
