Two functions in `starter.c` read arrays, and both cross the boundary by one.

`sum_array` should add `a[0]` through `a[n-1]`, but it also reads `a[n]`, so the total comes out too large. `last_value` should return the final element `a[n-1]`, but it returns whatever number sits just past the array.

Fix both spots.

**In plain words**: `sum_array` returns the sum of the first `n` elements, or `0` when `n` is `0`; `last_value` returns the element at index `n-1`. Both functions may touch only the `n` elements `a[0]` to `a[n-1]`.

**Interface**:

```c
int sum_array(const int *a, int n);
int last_value(const int *a, int n);
```

- `sum_array` returns `a[0] + ... + a[n-1]`; for `n == 0` it returns `0` and reads no element at all.
- `last_value` returns `a[n-1]`; callers pass an `n` of at least `1`.
- The slot right after `a` is not part of the array, so reading it means nothing.

### Example

```c
int data[5] = {3, 1, 4, 1, 5};
sum_array(data, 5);   /* 14 */
last_value(data, 5);  /* 5 */
```

### Contract

- Change only these two functions; do not write `main`.
- `n` is between `0` and `1000`, and every element lies between `-1000` and `1000`.
- Add each element at most once; do not count any twice.
