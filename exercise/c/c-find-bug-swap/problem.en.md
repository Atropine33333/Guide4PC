The program below reads two integers, swaps their values and prints the result — but what it prints is still the original order. Find and fix the mistake in `swap`.

**Input**: one line, two integers `a` and `b` separated by a single space, each with absolute value at most `10^9`.

**Output**: one line with the two integers after the swap, separated by a single space.

### Example

Input:

```text
3 7
```

Output:

```text
7 3
```

### Hints

- The two parameters of `swap` already are pointers; assigning to them only rewrites the local copies.
- When the two numbers are equal, the output looks the same before and after the swap.
- Keep the input and output format unchanged and do not rewrite the whole program; fix only the logic error in `swap` inside `starter.c`.
