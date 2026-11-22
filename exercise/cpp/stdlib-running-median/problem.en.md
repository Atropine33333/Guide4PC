Read `n` integers one at a time. After each insertion, print the **lower median** of all values seen so far: after sorting them ascending, the value at index `(m - 1) / 2`, where `m` is the current count (indices start at 0).

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 200000`).
- The second line contains `n` integers, each between `-10^9` and `10^9`.

**Output**: one line with `n` integers separated by single spaces; the `i`-th is the lower median after inserting the first `i` values.

### Example

Input:

```text
4
10 20 30 40
```

Output:

```text
10 10 20 20
```

After 10 the set is `[10]`, median 10; after 20 it is `[10, 20]`, lower median 10; after 30 it is `[10, 20, 30]`, middle value 20; after 40 it is `[10, 20, 30, 40]`, lower median 20.

### Constraints

- With an even count, take the lower of the two middle values, without averaging.
- Duplicate values count separately.
- Sorting after every insertion is too slow; maintain the median with two heaps or a balanced structure.
