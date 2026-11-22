[ ONE STATEMENT, TWO ANSWERS! ]

Still writing two loops to learn "what is 1 plus 2 up to `n`" and "how many terms was that"?

Meet the comma operator: it evaluates from left to right and keeps only the last value. So the step of a `for` loop can do three jobs at once — add the current term to the sum, add one to the count, and move to the next term.

One statement, both answers in hand.

**In plain words**: read `n`, then use comma expressions inside a single `for` statement to add the terms from `1` to `n` and count them, and print the sum and the count.

**Input**: one line with a single integer `n` (`0 ≤ n ≤ 1000000`).

**Output**: one line with two integers separated by a single space: the sum, then the count.

### Example

Input:

```text
100
```

Output:

```text
5050 100
```

### Contract

- The sum can exceed the range of `int`; keep it in a `long long` and print it with `%lld`.
- When `n` is `0`, both the sum and the count are `0`.
- Put the addition and the counting in one comma expression, evaluated left to right.
- No trailing spaces at the end of the line.
