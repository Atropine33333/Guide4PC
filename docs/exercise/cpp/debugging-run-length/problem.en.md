Read one non-empty line of lowercase letters and output its run-length encoding: each maximal run of the same character becomes "character + count", and the pieces are concatenated with no separators.

The provided starter compiles but produces wrong answers. Find and fix the bug without changing the input/output format.

**Input**: one line of lowercase letters, length between 1 and 200.

**Output**: one line with the run-length encoding.

### Example

Input:

```text
aaabbc
```

Output:

```text
a3b2c1
```

### Constraints

- Each run has at most 200 characters; writes counts in ordinary decimal without leading zeros.
- The last run must be printed too; do not drop it.
- Do not change the input/output format; only fix the bug in the starter.
