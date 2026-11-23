"MOVE-IT-YOURSELF" — ADVERTISEMENT

Tired of moving house? "Move-It-Yourself" launches a new service: slide a column of integers `k` places to the left and fill the `k` vacated slots at the back with zeros.

- **Overlapping haul**: the two stretches may touch, or even overlap; `memmove` does it in one trip and never tramples a number before it has moved.
- **One place or the whole column**: any `k` from `0` to `n` is accepted.
- **After the move**: zeros at the end, the books balanced.

**In plain words**: shift the first `n` elements of `a` left by `k`: `a[i]` takes the old `a[i + k]`, and the `k` slots left at the end are set to `0`; use `memmove` so overlaps are handled.

**Interface**:

```c
void shift_left(int *a, int n, int k);
```

- `n >= 0` and `0 <= k <= n`.
- `k` equals `0`: the array stays as it is.
- `k` equals `n`: all `n` elements become `0`.
- Otherwise: the first `n - k` elements move left and the last `k` become `0`.

### Example

```c
int a[6] = {1, 2, 3, 4, 5, 6};
shift_left(a, 6, 2);
// a becomes {3, 4, 5, 6, 0, 0}
```

### Contract

- Implement this function only; do not write `main`. These functions are called directly.
- When the regions overlap, a low-to-high loop reads numbers it has just written; let `memmove` do the whole move at once.
- Change only the first `n` elements; `a[n]` and the memory after it keep their values.
- No dynamic allocation.
