The program in `starter.c` should print the integers from `1` to `n`, separated by single spaces. It always prints one number too many: given `5`, it produces `1 2 3 4 5 6`. Fix that one spot.

**In plain words**: read `n` and print `1` through `n` in order, separated by single spaces. The loop in `starter.c` takes one step too many; fix it.

**Input**: one line with a single integer `n` (`1 ≤ n ≤ 30`).

**Output**: one line with the integers from `1` to `n` in order, separated by single spaces, with no trailing space.

### Example

Input:

```text
5
```

Output:

```text
1 2 3 4 5
```

### Contract

- Change only the faulty spot; keep the input and output formats as they are.
- `n` is at least `1`; empty input needs no handling.
- Print one line ending in a newline.
