A rule horror: the value waiting room

At the door of a waiting room you pick up a sheet titled *Rules for the Comparison Room*. Its edge is curled; the ink is still sharp:

Rule one. Before entering, leave your sign on the rack.
Rule two. A negative number puts on an unsigned coat; the number on the coat is the original plus 2 to the 32nd power.
Rule three. Line up in the coat. The queue recognises unsigned sizes only.
Rule four. On the way out, copy two things: who stands first (write `1` for yes, `0` for no), and the number on your coat.
Rule five. `-1` wears `4294967295`; it is larger than `1`. Do not be startled.

**In plain words**: read `int a` and `unsigned b`, then print two things — the truth of `(a < b)` (`1` for true, `0` for false) and the decimal value of `(unsigned)a` — separated by one space.

**Input**: one line with two integers `a` and `b`, where `-2147483648 ≤ a ≤ 2147483647` and `0 ≤ b ≤ 4000000000`.

**Output**: one line with two integers — the truth value of `(a < b)` and `(unsigned)a` — separated by one space.

### Example

Input:

```text
-1 1
```

Output:

```text
0 4294967295
```

`-1` becomes `4294967295` when converted to unsigned, which is not smaller than `1`, so the first item is `0`; the second item copies the number on the coat.

### Contract

- Here `unsigned` is 32 bits wide, with values from `0` to `4294967295`.
- Converting a negative number to unsigned wraps modulo `2^32`, so the result is well defined.
- Read `b` with `%u` and `a` with `%d`.
- No trailing spaces at the end of the line.
