(2 a.m. Channel 7. A saxophone wails.)

"Hi, folks, Ron here. Look at this number: `1234567`. Can you tell at a glance how big it is? Neither can I."

"But wait—look at this!"

(Ding!)

"The Comma-Matic 3000! From the right, every three digits—bam!—a comma. `1,234,567`. A million, clear as day."

"A minus sign? It stays right where it is. Nobody touches it. Leading zeros? The Comma-Matic 3000 doesn't do that. Zero is zero."

"Seen it on TV? You won't find it in any store. Call now—$19.99. But wait, there's more! The first ten callers get a free space-remover!"

"Operators are standing by. Act now."

**In plain words**: format each integer with thousands separators: insert a `,` every three digits from the right; keep a leading minus sign outside the grouping; output `0` for `-0`.

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 100`).
- The next `n` lines each contain one integer `x` (`-10^15 ≤ x ≤ 10^15`).

**Output**: `n` lines. Each is the decimal form of `x` with a comma inserted every three digits from the right. A minus sign stays at the front and is not grouped; `0` is printed as `0`.

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
- Print no leading zeros; `-0` must be printed as `0`.
- No spaces between groups.
