Three camps are mustered. A single word carries uppercase letters, lowercase letters and digits; sort them into the three camps and count each one.

**Input**: one line, a word without spaces, length between `1` and `1000`, containing only `A`–`Z`, `a`–`z` and `0`–`9`.

**Output**: one line with three integers — uppercase count, lowercase count, digit count — separated by single spaces.

### Example

Input:

```text
Hello123
```

Output:

```text
1 4 3
```

### Contract

- Compare character ranges directly; do not rely on the locale.
- No trailing spaces at the end of the line.
