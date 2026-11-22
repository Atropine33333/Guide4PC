This exercise reads no stdin. Implement the function; a hidden harness inspects the returned square.

```cpp
#include <vector>

std::vector<int> magic_square(int n);
```

Return an `n x n` square flattened in row-major order: it contains every integer from `1` to `n^2` exactly once, and every row, every column, and both main diagonals have the same sum.

Requirements:

- For positive odd `n`, construct any valid magic square.
- For negative `n`, `0`, or even `n`, return an empty `std::vector<int>`.
- The returned length must be exactly `n * n`, in row-major order.
- Do not write `main`.

### Hint

The Siamese method works: put `1` in the middle of the first row, then move up-right each time, wrapping around; if the target cell is already occupied, move one cell down from the current cell instead. For `n = 3`, one valid result is:

```text
8 1 6
3 5 7
4 9 2
```

### Constraints

- `0 <= n <= 15`, but only positive odd orders must produce a square.
- Each line sum is `n x (n^2 + 1) / 2`, which fits in `int`.
