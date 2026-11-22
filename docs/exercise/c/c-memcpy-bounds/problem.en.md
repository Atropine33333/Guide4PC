(At the edge of the freight yard a small cart stands with its bed divided into cells.)

A: How many cells does this cart hold?

B: The bed has `cap` cells in all, and the last one must stay free for the seal, so cargo may occupy at most `cap - 1` of them.

A: And if there are `n` items?

B: Load as many as fit. If the cargo outnumbers the room, load up to the cell before the seal; if the cargo is smaller, load it all. Then put the seal on the next cell.

A: And if there is not even one cell?

B: Then nothing is touched: no cargo loaded, no seal applied.

**In plain words**: implement `copy_capped(dst, cap, src, n)` to copy `n` bytes from `src` into `dst`, copying at most `cap - 1` bytes and writing a `'\0'` at the end; return the number of bytes copied, not counting the `'\0'`. When `cap` is `0`, write nothing and return `0`.

**Interface**:

```c
#include <stddef.h>

size_t copy_capped(char *dst, size_t cap, const char *src, size_t n);
```

- `dst` has at least `cap` writable bytes and `src` has at least `n` readable bytes.
- `cap == 0`: return `0` and leave every byte of `dst` unchanged.
- `cap > 0`: copy `min(n, cap - 1)` bytes, then write one `'\0'`, and return the number of bytes copied.
- Copying works byte by byte; a `'\0'` inside `src` is copied like any other byte, and copying stops after the `n`-th byte.
- The bytes of `dst` at index `cap` and beyond are not changed.

### Example

```c
char dst[4];
size_t r = copy_capped(dst, 4, "abcdef", 6);
/* the first four bytes of dst are 'a', 'b', 'c', '\0', and r is 3 */
```

The capacity is `4`, the seal takes the last cell, so cargo may occupy only three; there are six items, and only the first three are moved.

### Contract

- Implement only this function; do not write `main`.
- Use only standard library facilities.
- Never write past `dst`: the highest index written is `cap - 1`.
- Never read past `src`: the highest index read is `n - 1`.
- The return type is `size_t` and equals the number of bytes actually copied.
