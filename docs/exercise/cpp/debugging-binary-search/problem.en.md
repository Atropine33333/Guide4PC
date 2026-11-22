(A small stage. A folding fan and a wooden block on the table.)

A: Today I shall tell you about binary search. There is a row of numbers `values`, sorted from small to large, and we ask at which index the first value greater than or equal to `target` sits.

B: That is easy enough. Probe the middle: if it is below `target`, go right; if not, go left.

A: Sounds smooth to the ear. But the audience asked for the first one, and every position after the first one counts for nothing.

B: Where is the difference, then?

A: When `values[mid]` happens to equal `target`, you push `low` to `mid + 1`. The first position holding that value is waved goodbye, and you report the first position that is strictly greater.

B: Then I shall change it. On equality, close in on the left: set `high` to `mid` and keep `mid` inside the range.

A: Now you have it. Close it all the way down and `low` is the first position greater than or equal to the target.

B: And if every value is smaller than the target?

A: Then it stops at `values.size()` and nothing is left. The same with an empty array: it returns 0.

B: Very well; read the statement for the audience.

**In plain words**: implement `lower_bound_index(values, target)`: return the first index `i` with `values[i] >= target` in a non-decreasing array, or `values.size()` when every element is smaller. No stdin and no stdout; the hidden harness calls it directly. The starter pushes `low` to `mid + 1` on equality; narrow the right boundary and keep `mid` instead, leaving the signature alone.

Function:

```cpp
int lower_bound_index(const std::vector<int>& values, int target);
```

Return the first index `i` with `values[i] >= target`, or `values.size()` if every element is smaller than `target`.

Constraints:

- `values` is sorted in non-decreasing order and may be empty or contain duplicates;
- The return type is `int` in `0..values.size()`;
- Do not write `main`;
- Fix only the logic in `starter.cpp`; do not change the signature.

Hint: the current implementation moves `low` to `mid + 1` when `values[mid] == target`, so it returns the first position greater than `target` instead of the first greater-or-equal position.
