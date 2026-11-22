Read a list of dot-separated version numbers and print them sorted from smallest to largest, **stably**.

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 100`).
- The next `n` lines each contain a version: 1 to 4 non-negative integers joined by `.`, each between `0` and `10^9`; leading zeros are allowed.

**Output**: `n` lines with the sorted versions, preserving their original textual form (including leading zeros).

### Ordering rules

- Compare numeric components from left to right; the first differing component decides the order.
- When versions have different lengths, missing components count as 0: `1.2`, `1.2.0`, and `1.02.0` are equal.
- Equal versions keep their input relative order (stable sort).
- For example `1.2.3 < 1.2.10 < 1.10.0`.

### Example

Input:

```text
3
1.2.3
1.10.0
1.2.10
```

Output:

```text
1.2.3
1.2.10
1.10.0
```

### Constraints

- Do not sort the raw strings: `"1.10"` would come before `"1.2"`.
- Leading zeros affect only the printed form, not the comparison: `01.002` equals `1.2`.
- Each component is at most `10^9`; comparing with 64-bit integers is safer.
- Do not let an unstable `std::sort` reorder equal versions.
