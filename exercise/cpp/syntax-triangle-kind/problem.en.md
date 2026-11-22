Read three integer side lengths and classify the triangle they form.

**Input**: one line with three integers `a b c` (`1 ≤ a, b, c ≤ 10^9`).

**Output**: exactly one of:

- `NOT_TRIANGLE`: the sides cannot form a triangle (including the degenerate case where two sides add up to the third).
- `EQUILATERAL`: all three sides are equal.
- `ISOSCELES`: exactly two sides are equal.
- `SCALENE`: all three sides are different.

### Example

Input:

```text
3 4 5
```

Output:

```text
SCALENE
```

### Constraints

- Every pair of sides must sum to strictly more than the third side; checking one pair is not enough.
- The sum of two sides may exceed the range of `int`; decide with 64-bit integers.
- Check the triangle condition before comparing side lengths.
