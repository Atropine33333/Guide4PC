Now there are several versions, each divided by dots, such as `1.2.3`. How shall they be ordered?

Method: compare the parts from left to right. Where a version has fewer parts, fill with zero. The greater number goes later, the lesser earlier. If every part is equal, the two versions are equal, and their original order shall be kept.

Again: a version has one to four parts, and each part is at most `10^9`; leading zeros may be written, and they take no part in the comparison, remaining only on the page. Thus `1.2`, `1.2.0`, and `1.02.0` are equal.

A questioner asked: may they be compared as text? Answer: no. As text, `1.10` comes before `1.2`; turn them into numbers to obtain the order.

Now take three versions: `1.2.3`, `1.10.0`, `1.2.10`. Method: `1.2.3` first, `1.2.10` next, `1.10.0` last.

**In plain words**: Read `n` dot-separated versions (1 to 4 parts, each from 0 to `10^9`, leading zeros allowed), compare part by part numerically, and sort stably; print each version in its original textual form. Use 64-bit integers and keep the input order for equal versions.

### Input

- The first line contains an integer `n` (`1 ≤ n ≤ 100`).
- The next `n` lines each contain a version: 1 to 4 non-negative integers joined by `.`, each between `0` and `10^9`; leading zeros are allowed.

### Output

`n` lines with the sorted versions, preserving their original textual form (including leading zeros).

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
