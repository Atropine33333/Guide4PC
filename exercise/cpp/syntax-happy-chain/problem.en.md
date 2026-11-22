Repeatedly replace a positive integer `n` with the sum of the squares of its decimal digits.

For example, starting from `19`: `19 -> 82 -> 68 -> 100 -> 1`, reaching 1 after 4 replacements.
Starting from `2` enters the cycle `4 -> 16 -> 37 -> 58 -> 89 -> 145 -> 42 -> 20 -> 4` and never reaches 1.

Find the number of replacements needed to reach 1; output `-1` if 1 is never reached.

**Input**:

- One integer `n` on a single line (`1 <= n <= 10^12`).

**Output**:

- One integer: the number of replacements, or `-1`.

### Example

Input:

```text
19
```

Output:

```text
4
```

### Constraints

- The replacement count for `1` is `0`.
- Only cycle detection is required; do not print the path.
- A digit-square sum never exceeds `12 x 81 = 972`; `long long` is enough for the input.
