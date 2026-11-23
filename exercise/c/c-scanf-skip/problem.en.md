One line carries two assignments, with spaces and commas thrown in wherever they happen to land. Do not count them — let `scanf` skip the noise and read the two integers.

**Input**: one line shaped like `a=<integer> b=<integer>`. Spaces are allowed between the letters `a`, `b` and the `=`, and between `=` and the digits. The two assignments are separated by one or more spaces, by commas, or by a mix of both. Both integers may be negative and have absolute value at most `10^9`.

**Output**: one line with a single integer, the sum of the two values.

### Example

Input:

```text
a = 12 , b = -30
```

Output:

```text
-18
```

### Contract

- Read both integers with a single `scanf` call.
- The separator's length and kind vary; do not hard-code a fixed number of spaces or commas in the format string.
- The sum stays well inside the `int` range.
- No trailing spaces and no text beyond the required line.
