Maintain a multiset of integers that supports insertion, deletion of one element, and a query for the `k`-th smallest value.

**Input**:

- The first line holds one integer `q` (`1 <= q <= 2000`), the number of operations.
- The next `q` lines each contain one of:
  - `add x`: insert one `x` (`|x| <= 10^9`);
  - `erase x`: remove one element equal to `x`; do nothing if none exists;
  - `kth k`: query the `k`-th smallest element (`k` is an ordinary integer and may be less than `1` or larger than the current count).

**Output**:

- For every `kth` operation, print one line: the `k`-th smallest element, or `none` when `k < 1` or `k` exceeds the current count.
- Other operations produce no output.

### Example

Input:

```text
6
add 5
add 1
add 5
kth 1
erase 5
kth 2
```

Output:

```text
1
5
```

### Constraints

- Duplicate values are allowed; `erase` removes just one copy.
- Queries are not nested; process them in input order.
- The `k` of a `kth` query may be `0` or negative.
- Output contains only lowercase letters and decimal digits.
