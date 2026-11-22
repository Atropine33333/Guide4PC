This exercise reads no stdin and writes no stdout. The binary search below contains a bug; fix it so the hidden harness passes.

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
