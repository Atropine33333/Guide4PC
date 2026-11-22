A: Let me ask you something. What is `-15 / 2`?

B: Do I even need to think? `-7`. Every computer says so.

A: The computer rounds toward zero. Mathematics also has a rounding-down, and that one gives `-8`.

B: So? Off by one, who cares?

A: I care! For positive numbers the two agree, but the moment you touch a negative and the division leaves a remainder, they part ways by exactly 1. Stir both averages into one pot and the number you report is a slander.

B: Then what do we do?

A: Keep the total in a 64-bit integer, divide, then look at the remainder. Remainder nonzero and the quotient negative? Subtract one more.

B: So the remainder has the final word.

A: Right. The remainder has the final word. And don't try to patch it with floating point; it won't hold.

**In plain words**: read `n` signed integers and print two averages: the truncating average `sum / n` in C++ (rounding toward zero) and the mathematical floor `⌊sum / n⌋`. They agree when the division is exact and differ by 1 only when the total is negative and the division leaves a remainder.

**Input**: the first line contains an integer `n` (`1 ≤ n ≤ 1000`). The second line contains `n` integers, each with absolute value at most `10^9`.

**Output**: one line with two integers — the truncating average and the floor average — separated by a single space.

### Example

Input:

```text
2
-7 -8
```

Output:

```text
-7 -8
```

`-15 / 2` is `-7` in C++, while the floor is `-8`.

### Constraints

- Keep the total in a 64-bit integer.
- Do not compute the average with floating point.
