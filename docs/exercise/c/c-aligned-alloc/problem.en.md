THE ALIGNED CELLS

The fortune teller spreads a cloth: eight cells across, eight down, sixty-four in all. The cards must sit straight. The starting edge of every card has to lie on a grid line, and being one cell off does not count.

A visitor names a number of implements, and the teller cuts a patch from the cloth large enough to hold them. The starting edge of that patch always lands on a grid line. The teller notes the cut in the corner, and when the table is cleared the cloth is returned according to that note.

If someone wants nothing at all, the teller shakes her head and holds out empty hands.

**In plain words**: implement `alloc_aligned64(n)` to obtain at least `n` bytes whose start address is a multiple of 64; `free_aligned64` gives that patch back. When `n` is 0, return `NULL`.

**Interface**:

```c
#include <stddef.h>
#include <stdint.h>

void *alloc_aligned64(size_t n);
void free_aligned64(void *p);
```

- `n > 0`: return a non-`NULL` pointer with `(uintptr_t)pointer % 64 == 0`, and `n` readable and writable bytes starting there; return `NULL` if the memory cannot be obtained.
- `n == 0`: return `NULL`.
- `free_aligned64` accepts only pointers returned by `alloc_aligned64`; `free_aligned64(NULL)` does nothing.
- Two allocations never overlap.
- The returned address may differ from the address `malloc` handed out; `free_aligned64` finds the original block when the patch is returned.

### Example

```c
#include <string.h>

void *p = alloc_aligned64(100);   /* p is a multiple of 64 */
memset(p, 0, 100);
free_aligned64(p);
```

### Contract

- Implement only these two functions; do not write `main`.
- Use only the standard library and handle alignment yourself with `malloc`; do not rely on an aligned allocation function from the runtime.
- The alignment requirement is fixed at 64.
- An accidentally aligned address from `malloc` does not count; the result must hold on another machine and on another run.
- Do not use the pointer after it is returned, and return each pointer exactly once.
