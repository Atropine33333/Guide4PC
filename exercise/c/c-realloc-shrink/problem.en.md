Shrink a dynamically allocated array: keep only the first `n` elements and change the capacity from `*cap` to `n`. When `n` equals the current capacity, do nothing; when `n` is `0`, return the whole block; when `n` is smaller, shrink and keep the first `n` values; when `n` is negative or larger than the capacity, refuse and leave the array untouched.

**In plain words**: implement `trim_to`, which shrinks a dynamic array's capacity from `*cap` to `n` while keeping the elements at indices `0` through `n-1`; when `n` is `0`, return the whole block; when `n` is negative or larger than the current capacity, return `-1` and change nothing.

**Interface**:

```c
int trim_to(int **a, int *cap, int n);
```

- `*a` is the first element's address, `*cap` is the current capacity, and indices `0` through `*cap - 1` are readable and writable.
- `0 < n < *cap`: shrink the capacity to `n`, keep the elements at indices `0` through `n-1` with their values and order, return `0`.
- `n == *cap`: capacity unchanged, return `0`.
- `n == 0`: return the whole block, set `*a` to `NULL` and `*cap` to `0`, return `0`.
- `n < 0` or `n > *cap`: return `-1`; `*a`, `*cap` and all elements stay as they were.
- Shrinking fails: return `-1`; the original array is still usable.

### Example

With capacity `8` holding `0` through `7`, `trim_to(&a, &cap, 4)` returns `0`, the capacity becomes `4`, and indices `0` through `3` still hold `0`, `1`, `2`, `3`. Calling `trim_to(&a, &cap, 0)` next zeroes the capacity and makes `a` `NULL`.

### Contract

- Implement only this function; do not write `main`.
- Shrink with `realloc`; the first `n` elements must keep their values.
- After the whole block is returned, the caller's pointer is already `NULL`.
- When the function returns `0`, `*cap` equals `n`; when it returns `-1`, all three values are unchanged.
