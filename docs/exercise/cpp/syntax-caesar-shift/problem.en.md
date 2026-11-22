"Still sending your letters in plain text?"

On late-night television the host raises a small brass wheel. "The Caesar Wheel! Two thousand years of Roman generals, and today it can be yours for just $19.99! Turn every letter `k` places forward: `z` rolls over to `a`, `Z` rolls over to `A`; spaces, punctuation and digits all stay where they are."

"But wait, there's more! A negative `k` turns the wheel backwards, and a hundred million turns it no further than twenty-six — take the remainder first and fear no `k` at all! And read the whole line as it comes, not one word with `std::cin >> word`!"

"While stocks last. Letters, ledgers, notes to your sweetheart — one wheel in hand and nothing can go wrong."

**In plain words**: read a shift `k` and one line of text, rotate every English letter forward by `k` within its own case range, leave every non-letter untouched, and print the line.

**Input**:

- The first line holds one integer `k` (`-10^9 ≤ k ≤ 10^9`); a negative value shifts backwards.
- The second line holds one line of text no longer than 100 characters, ASCII 32 to 126 inclusive (spaces and punctuation included).

**Output**: one line, the shifted text. Leading, trailing and inner spaces must all be preserved exactly.

### Example

Input:

```text
3
Attack at dawn!
```

Output:

```text
Dwwdfn dw gdzq!
```

### Constraints

- Only `a`–`z` and `A`–`Z` change.
- The shift may far exceed 26, so take it modulo 26 first; after a negative modulo the shift must still be non-negative.
- Read the whole line with `std::getline`; `std::cin >> word` alone is not enough.
