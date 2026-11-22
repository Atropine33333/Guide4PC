Read several money amounts, sum them exactly in cents, and print the total with two decimals.

**Input**:

- The first line holds an integer `n` (`1 ≤ n ≤ 1000`).
- The next `n` lines each hold one amount: an optional minus sign, digits, and optionally a dot followed by 1–2 fraction digits. The absolute value is at most `10^9`.

**Output**: one line with the total formatted with exactly two decimals (for example `-0.50`, `3.30`, `1000000000.00`). A zero total prints `0.00`, never `-0.00`.

### Example

Input:

```text
2
1.10
2.20
```

Output:

```text
3.30
```

### Constraints

- Do not accumulate in `double`: values such as `0.1 + 0.2` are not exactly representable and the printed total may be off by one cent.
- Parse every amount into integer cents, accumulate in `long long`, and format the result as `units.cents`.
- The fraction may have a single digit (`0.1` means `0.10`) or be absent (`3` means `3.00`).
