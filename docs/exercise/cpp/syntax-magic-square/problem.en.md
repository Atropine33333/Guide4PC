> Out of the Luo River came the chart;
> three roads through every square run flat —
> across, along, and cornerwise —
> and each one sums to fifteen wise.

The ancients had the Luo Shu: one at the bottom centre, nine at the top, and every row, column and diagonal summing to fifteen alike. Now there is a square field of side `n`, to be filled with the numbers `1` through `n²`, each exactly once, so that every row, every column and both main diagonals have the same sum.

**In plain words**: implement `magic_square(n)`, returning an `n × n` magic square flattened in row-major order; for positive odd `n` build any valid one, for every other `n` return an empty vector.

```cpp
#include <vector>

std::vector<int> magic_square(int n);
```

Return the `n × n` square flattened by rows: it contains every integer from `1` to `n²` exactly once, and each row, each column and both main diagonals have equal sums.

**Requirements**:

- For positive odd `n`, build any valid magic square.
- For negative, `0` or even `n`, return an empty `std::vector<int>`.
- The returned length must be exactly `n * n`, in row-major order.
- Do not write `main`.

### Example

For `n = 3`, one possible result is:

```text
8 1 6
3 5 7
4 9 2
```

### Constraints

- `0 ≤ n ≤ 15`, but by convention only positive odd `n` get a square.
- The magic sum equals `n × (n² + 1) / 2` and fits in `int`.

### The rule of the rods

Use the Siamese method: place `1` in the middle of the first row, then step one cell up and one cell right each time, wrapping around at the borders; if the target cell is already taken, drop one cell down from the current position instead.
