The program below tries to find the most frequent value in a list of integers, breaking ties by choosing the smaller value; it picks wrong answers for ties and for inputs where every value is distinct. Fix `starter.cpp`.

**Input**:

- The first line holds an integer `n` (`1 ≤ n ≤ 10^5`).
- The second line holds `n` integers with absolute value at most `10^9`.

**Output**: one integer, the most frequent value; when several values tie, print the smallest one.

### Example

Input:

```text
4
1 1 2 2
```

Output:

```text
1
```

### Constraints

- Values may be negative as well as 0.
- Many candidates may share the maximum frequency, so the smallest one must win.
- Do not change the input/output format or rewrite it as a completely different program.
- Only modify `starter.cpp`.
