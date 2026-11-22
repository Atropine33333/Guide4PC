Read a signed integer `a` and an unsigned integer `b`, and compare them by their mathematical values rather than by C++'s implicit conversion.

**Input**:

- The first line contains an integer `a` from `-9223372036854775808` to `9223372036854775807`.
- The second line contains an integer `b` from `0` to `18446744073709551615`.

**Output**: two `0` or `1` values on one line, separated by a single space: the first says whether `a < b`, the second whether `a == b`, both by mathematical value.

### Example

Input:

```text
-1
0
```

Output:

```text
1 0
```

### Constraints

- A direct `a < b` or `a == b` converts `a` to unsigned, turning a negative value into a huge number and giving the wrong answer.
- When `a` is negative it is always smaller than any non-negative `b`.
- Do not compare through floating point: `b` is beyond the exact range of `double`.
