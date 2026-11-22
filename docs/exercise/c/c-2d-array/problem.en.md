A two-dimensional mat has to be laid out in memory. C offers no ready-made 2-D array whose rows you can size separately, so you first get a row of row pointers and then carve out a separate patch for each row. When the mat is laid, every cell holds `0`; rows keep to themselves, and touching one row never disturbs another.

**In plain words**: `make2d(rows, cols)` obtains `rows` pointers to `int`, then obtains `cols` `int`s for every row and sets them all to `0`; `sum2d` adds up the whole mat; `free2d` gives the rows back one at a time, in the reverse of the order they were obtained.

**Interface**:

```c
int **make2d(int rows, int cols);
long long sum2d(int **grid, int rows, int cols);
void free2d(int **grid, int rows);
```

- If `rows` or `cols` is not positive, `make2d` returns `NULL`.
- If an allocation fails, return `NULL`, giving back everything already obtained.
- Each row is a separate allocation; rows share no storage.
- `sum2d` returns the total as `long long` so that summing cannot overflow `int`; it returns `0` when `grid` is `NULL`.
- `free2d` returns each row and then the array of row pointers; when `grid` is `NULL` it does nothing.

### Example

```c
int **g = make2d(2, 3);   /* 2 rows, 3 columns, six zeros */
g[0][0] = 7;
g[1][2] = -2;             /* each cell is its own */
long long s = sum2d(g, 2, 3);   /* 5 */
free2d(g, 2);
```

### Contract

- Implement only these three functions; do not write `main`.
- Each row is obtained separately; do not fake several rows with one contiguous block.
- Give a mat back exactly once; a new mat may be obtained afterwards.
