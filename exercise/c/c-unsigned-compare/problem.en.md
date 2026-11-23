THE RULES OF GOING DOWN

At night the building only goes down. Four rules are taped to the stairwell:

1. The slip of paper holds one integer `n`, `1 ≤ n ≤ 10`. If there is no slip, do not go down.
2. Count downwards from `n`: `n`, `n-1`, all the way to `1`. Do not skip a single number.
3. Put exactly one space between neighbours, and stop the moment you say `1`. This building has no floor `0`.
4. Do not read rule four. You are already on your way down.

**In plain words**: read the integer `n` and print every number from `n` down to `1`, separated by single spaces.

**Input**: one line with an integer `n`, `1 ≤ n ≤ 10`.

**Output**: one line holding the numbers from `n` down to `1`, separated by single spaces, with no trailing space.

### Example

Input:

```text
5
```

Output:

```text
5 4 3 2 1
```

### Contract

- `n` is at least `1`, so `1` is always printed.
- Do not print a trailing space at the end of the line.
- Print nothing besides that one line of numbers.
