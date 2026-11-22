Read `n` integers and print each with thousands separators.

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 100`).
- The next `n` lines each contain one integer `x` (`-10^15 ≤ x ≤ 10^15`).

**Output**: `n` lines. Each line is the decimal form of `x` with a comma `,` inserted every three digits from the right. The minus sign stays at the front and is not grouped; `0` prints as `0`.

### Example

Input:

```text
3
1234567
-9876543
0
```

Output:

```text
1,234,567
-9,876,543
0
```

### Constraints

- Do not assume a fixed number of digits; inputs may have 16 digits.
- Do not print leading zeros; `-0` must print as `0`.
- Do not put spaces between groups.
