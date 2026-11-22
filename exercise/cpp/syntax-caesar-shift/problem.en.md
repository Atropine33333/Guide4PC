Read an integer `k` and one line of text, then apply a Caesar shift to every English letter: move it `k` positions forward within its own letter case, wrapping past `z` or `Z`. Non-letter characters stay unchanged.

**Input**:

- The first line contains an integer `k` (`-10^9 ≤ k ≤ 10^9`); a negative value means shifting backward.
- The second line contains text of length at most 100, with ASCII codes from 32 to 126 (spaces and punctuation allowed).

**Output**: one line with the shifted text. Leading, trailing, and internal spaces must be preserved exactly.

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
- The shift may be much larger than 26; reduce it modulo 26 first, and make a negative residue non-negative.
- Read the whole line with `std::getline`, not with `std::cin >> word`.
