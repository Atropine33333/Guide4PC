(A small stage. A folding fan and a wooden block rest on the table.)

A: Today we sort a pile. We are given `n` numbers and a pivot `k`, and we divide the numbers into three groups by `k`.

B: That I know: those less than `k` go left, those greater than `k` go right, and the middle stays empty.

A: The middle does not stay empty. Those equal to `k` go in the middle. Less, equal, greater: three groups.

B: Three groups, then. What about who came first?

A: Within each group, whoever stood ahead still stands ahead. That is a stable partition. Take `3 1 4 2 6 5` with `k = 3`: below 3 we have `1 2`, equal to 3 we have `3`, above 3 we have `4 6 5`; join the three groups in their own orders and you get `1 2 3 4 6 5`.

B: And if `k` is absent from the sequence?

A: We partition all the same. The middle group is empty; the other two keep their order. `n` may reach a hundred thousand, so do not compare every pair.

B: Right. One line of output, one space between numbers, and no extra space at the end.

A: Also, the standard library only.

B: You do have a great many rules.

**In plain words**: Given an integer sequence of length `n` and a pivot `k`, stably partition it into three parts: values below `k`, values equal to `k`, and values above `k`, preserving the original relative order within each part. Print one line of `n` integers separated by single spaces, with no trailing space. `n` may be 100000, and only the standard library may be used.

### Input

- The first line holds `n` and `k` (`1 ≤ n ≤ 100000`, `-10^9 ≤ k ≤ 10^9`).
- The second line holds `n` integers, `-10^9 ≤ a[i] ≤ 10^9`.

### Output

One line with `n` integers separated by single spaces, with no trailing space.

### Example

Input:

```
6 10
5 12 3 10 7 10
```

Output:

```
5 3 7 10 10 12
```

### Constraints

- The partition must be stable: for `3 1 4 2 6 5` with `k = 3`, the values below 3 (`1 2`) come first in order, then the value equal to 3, then the values above 3 (`4 6 5`) in order.
- Standard library only.
