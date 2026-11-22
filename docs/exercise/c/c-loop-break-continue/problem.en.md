Count from `1` up to `n`, skipping every multiple of `3`, and add the rest into one running total. As soon as the total passes `100`, stop at once and look at nothing after it.

**In plain words**: read `n`, add every number from `1` through `n` that is not a multiple of `3`; the moment the running total is greater than `100`, stop immediately; print the final total.

**Input**: one line with an integer `n` such that `0 ≤ n ≤ 1000`.

**Output**: one line with the resulting total.

### Example

Input:

```text
6
```

Output:

```text
12
```

`1 + 2 + 4 + 5 = 12`; `3` and `6` were skipped.

Input:

```text
17
```

Output:

```text
108
```

At `17` the total passes `100` for the first time, so it stops at `108`.

### Contract

- Use `continue` to skip and `break` to stop; check the total right after each addition.
- For `n = 0` nothing is added, so the output is `0`.
- The total stops growing past `100`, so an `int` is enough.
- No trailing spaces at the end of the line.
