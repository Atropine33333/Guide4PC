You are given two non-decreasing integer sequences, possibly containing duplicates. Treat each sequence as a set and output the union, the intersection, and the difference (the first sequence minus the second), each sorted ascending with duplicates removed.

### Input

- The first line holds two integers `n` and `m` (`0 ≤ n, m ≤ 100000`), the lengths of the sequences.
- The second line holds `n` non-decreasing integers, forming sequence A.
- The third line holds `m` non-decreasing integers, forming sequence B.

When a length is 0, its line is empty.

### Output

Three lines: the union, the intersection, and A minus B. Integers in each line are ascending, separated by one space. If the result is empty, print `empty`.

### Example

Input:

```
3 3
1 3 5
2 3 4
```

Output:

```
1 2 3 4 5
3
1 5
```

### Constraints

- Both sequences are already non-decreasing.
- A sequence may contain duplicates; each value appears once in the output.
- `n + m` can reach `200000`; do not compare every pair.
