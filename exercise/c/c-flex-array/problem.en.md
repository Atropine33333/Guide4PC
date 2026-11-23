(Statute style, excerpted from the Plot Requisition Code.)

Article 1. Anyone who needs a row of integers applies to the camp. The camp stakes out the plot in one go for a count of `n`: the count sits at the head, `n` integers follow it, and no spare ground is left at the end.

Article 2. In a newly staked plot, every cell is zero.

Article 3. Values may be moved in from elsewhere, and the count must match the source; an empty source may only supply zero of them.

Article 4. Reading a cell requires its index. An index out of range yields `0`.

Article 5. Writing a cell also requires its index. An index out of range is refused and the plot stands as it was.

Article 6. Sums are kept in `long long`, so that nothing overflows along the way.

Article 7. Leaving the camp returns the whole plot; leaving empty-handed is no offence.

**In plain words**: the last line of `IntFlex` is `int a[];`, so the number of cells is not stored in the struct; obtain the whole thing at once with `sizeof(IntFlex) + n * sizeof(int)`, keep `n` at the head, and let the cells follow.

**Interface**:

```c
typedef struct {
    size_t n;
    int a[];
} IntFlex;

IntFlex *flex_new(size_t n);
IntFlex *flex_from(const int *src, size_t n);
int flex_get(const IntFlex *v, size_t i);
void flex_set(IntFlex *v, size_t i, int value);
long long flex_sum(const IntFlex *v);
void flex_free(IntFlex *v);
```

- `flex_new` obtains `sizeof(IntFlex) + n * sizeof(int)` bytes, stores `n`, and sets every cell to `0`; with `n` equal to `0` it still returns a valid object; on failure it returns `NULL`.
- `flex_from` makes `n` cells and moves the values of `src` in one by one; it returns `NULL` when `src` is `NULL` and `n` is not `0`.
- `flex_get` returns the value of cell `i`; it returns `0` when `i` is not below `n` or `v` is `NULL`.
- `flex_set` writes cell `i`; out of range it does nothing.
- `flex_sum` returns the total of all cells, `0` for an empty plot.
- `flex_free` returns the whole plot; `flex_free(NULL)` does nothing.

### Example

```c
int src[3] = {3, -1, 7};
IntFlex *v = flex_from(src, 3);   /* n == 3, cells are 3, -1, 7 */
long long s = flex_sum(v);        /* 9 */
flex_set(v, 1, 10);               /* cells become 3, 10, 7 */
flex_set(v, 3, 0);                /* out of range, refused */
flex_free(v);
```

### Contract

- Implement only these six functions; do not write `main`.
- One request, one return; do not allocate the cell array separately.
- Keep the last line of the struct as `int a[];` so the cells live in the same block as `n`.
