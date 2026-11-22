DOUBLE-PRECISION COMPARATOR, MODEL G4, OPERATING MANUAL

1. Purpose

The instrument compares two double-precision floating-point numbers and reports two readings, one per line.

2. Installation

None required. Hand it the two numbers and it starts working.

3. First reading (tolerance comparison)

Take the absolute value of the difference. When it is below `1e-9` the numbers count as equal and the reading is `1`; otherwise the reading is `0`.

4. Second reading (self-test)

The instrument computes `0.1 + 0.2` and compares it with `0.3` using `==`. Equal gives `1`, otherwise `0`. This reading only reflects the representation error of double precision and does not depend on the two numbers handed in.

5. Reading order

The first reading on top, the second below, one reading per line, each a `0` or a `1`.

6. Cautions

- Do not compare the two input numbers with `==`.
- Work out the sign yourself when taking the absolute difference; no extra math library is needed.
- Inputs may be negative, zero or fractional, and both magnitudes stay within `10^9`.

**In plain words**: read two `double` values, print the result of `|a - b| < 1e-9` (`1` or `0`) on the first line, then the result of `0.1 + 0.2 == 0.3` (`1` or `0`) on the second line.

**Input**: one line with two decimal numbers `a` and `b`, separated by a single space, each of magnitude at most `10^9`.

**Output**: two lines, each holding a single `0` or `1`. The first line is the tolerance comparison; the second is the self-test.

### Example

Input:

```text
1.0 1.0000000005
```

Output:

```text
1
0
```

The two numbers differ by about `5e-10`, less than `1e-9`, so the first reading is `1`; in double precision `0.1 + 0.2` is `0.30000000000000004`, which is not equal to `0.3`, so the second reading is always `0`.

### Contract

- The tolerance test uses `<`; a difference of exactly `1e-9` gives `0`.
- The second reading does not depend on the input and is always `0`.
- Exactly two lines, each terminated by a newline, no blank lines.
- Print nothing beyond the answer.
