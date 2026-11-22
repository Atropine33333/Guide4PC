"The kingdom of heaven is like a traveller carrying a line of words, with a line width of `W`. At every village he takes into his basket as many whole words as will fit, and leaves the rest for the next village; never does he split a word in two."

"And it is written: if a word is longer than the basket, it walks a stretch alone, and no one may fold it."

"Walk in the original order, and do not look back to rearrange. When you measure the basket, forget not the single space between words—it takes room as well."

"And again: after the width `W` is read, the newline still waits in the stream; dismiss it with a `std::getline` or an `ignore` before you receive the line of words."

**In plain words**: read a width `W` and one line of lowercase words, and wrap greedily: each line holds as many whole words as fit, one space between words, no leading or trailing spaces; a word longer than `W` takes a line of its own.

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
