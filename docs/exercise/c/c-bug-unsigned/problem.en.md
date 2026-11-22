NOTICE FOR STAIRWELL 13

1. This building is numbered from the top down: the highest step is `n`, the lowest is `1`.
2. With each step down, call out the number under your feet. Leave one space between numbers.
3. Once `1` has been called, stop and start a new line; there is no need to turn back.
4. There is no step zero, and no negative step. If someone keeps walking down and never reaches the bottom, he has counted past the end.
5. If you hear footsteps counting down from `n` without end, do not count along.

**In plain words**: read `n` and print `n, n-1, ..., 1` on one line, separated by single spaces, ending with a newline. The loop in `starter.c` counts with an `unsigned` value, so `i >= 0` is always true and it never stops after `1`. Fix that condition.

**Input**: one line with an integer `n` (`1 ≤ n ≤ 1000`).

**Output**: one line with the integers from `n` down to `1`, separated by single spaces.

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

- `n` is at least `1`; there is no step zero or below.
- The last number is followed directly by a newline; no trailing spaces.
- Change only the broken spot; keep the input and output format as it is.
