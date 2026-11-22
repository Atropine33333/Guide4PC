**The Triangle Gallery: Rules for Visitors**

1. Every exhibit in this gallery is made of three integer side lengths, listed on one line with spaces between them.

2. Of the three sides, any two must sum to strictly more than the third. Where two sides sum exactly to the third, the piece is a "degenerate" thing of inferior grade; the gallery will not display it and labels it `NOT_TRIANGLE`.

3. Three equal sides are hung with the `EQUILATERAL` label; exactly two equal sides with the `ISOSCELES` label; three unequal sides with the `SCALENE` label.

4. Before any label is hung, rule 2 must be checked. Where rule 2 fails, rules 3 and 4 are automatically void.

5. Sides may be as long as `10^9`. Sum them in 32-bit integers and rule 2 will return a false verdict — use 64 bits here.

6. Every rule above is true; only common sense must give way to order in this gallery.

**In plain words**: read three integer side lengths, first decide whether they form a triangle (degenerate cases do not), then print one of four labels by how many sides are equal: `NOT_TRIANGLE`, `EQUILATERAL`, `ISOSCELES`, `SCALENE`.

**Input**: one line holding three integers `a b c` (`1 ≤ a, b, c ≤ 10^9`).

**Output**: one of the following four:

- `NOT_TRIANGLE`: no triangle (including the degenerate case where two sides sum exactly to the third).
- `EQUILATERAL`: all three sides equal.
- `ISOSCELES`: exactly two sides equal.
- `SCALENE`: all three sides different.

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

- Every pair of sides must sum to strictly more than the third; checking one pair alone is not enough.
- A sum of sides may exceed the `int` range; test in 64-bit integers.
- Decide whether a triangle exists first, then compare the sides.
