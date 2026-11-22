Two version numbers sit side by side in the terminal. They are compared segment by segment: from left to right, the first segment that differs decides the order; a missing segment counts as `0`. Numbers inside a segment are compared by decimal value, so `10` is larger than `9`.

**Input**: two lines, one version number each. A version contains only digits and `.`, has `1` to `4` segments separated by single dots, and each segment is an integer from `0` to `1000000000`; leading zeros are allowed.

**Output**: one line holding `-1`, `0` or `1`: `-1` when the first line is smaller, `0` when they are equal, and `1` when the first line is larger.

### Example

Input:

```text
1.2
1.2.0
```

Output:

```text
0
```

Input:

```text
1.10
1.9
```

Output:

```text
1
```

### Contract

- Treat every version as having 4 segments and fill missing ones with `0`.
- Compare each segment by integer value, never as a string.
- Versions contain only digits and `.`; no empty segment, sign or other character appears.
- A line may or may not end with a newline.
- No trailing spaces at the end of the output line.
