Read `n` floating-point numbers and print each on its own line: two digits after the decimal point, rounded, right-aligned, with a total field width of 10 characters. Pad with spaces on the left when the text is shorter; when it is longer, print it in full without truncation.

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 100`).
- The next `n` lines each contain one floating-point number with absolute value at most `10^9`.

**Output**: `n` formatted lines.

### Example

Input:

```text
3
1.5
-2.25
3.14159
```

Output (dots show spaces for readability):

```text
······1.50
·····-2.25
······3.14
```

In the real output the dots are ordinary spaces.

### Constraints

- Always print exactly two decimal digits, including `.00` for whole numbers.
- Let the output formatter round; do not add `0.005` yourself.
- The width of 10 is a minimum for the whole line; the sign and the decimal point count toward it.
- Do not print tabs, and do not add extra blank lines.
