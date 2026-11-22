Oh, my dear, at last someone who knows the trade. Do sit down—consider this your own cutting room.

You see, on the scripts this old machine receives, the backslash is a meddlesome fellow. The moment it appears, the letter after it must change its voice: `n` is read as a newline, `t` as a tab, `r` as a carriage return; and a backslash standing shoulder to shoulder with another becomes one lonely backslash. Good heavens, what a trade for rules.

And when some other character follows it? Leave both exactly as they are, and let no one touch either. If it stands at the very end of the script with nobody behind it, so be it—leave the one backslash where it is. Spaces, digits, punctuation: copy them all.

And please, my dear, do not write your own `main`.

**In plain words**: implement `expand_escapes(const std::string& input)`; scan left to right, expanding `\n`, `\t`, `\r` and `\\` into the characters they stand for, and keeping unknown escapes and a lone trailing backslash as they are.

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
