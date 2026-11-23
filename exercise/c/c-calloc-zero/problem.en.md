(A small stage. A row of mats lies on the floor; A crouches beside it.)

A: Your mats are laid out neatly, one against the next, and every one looks clean.

B: Fresh stock. Tell me how many you want and I lay out that many; each one comes up clean, not a speck of dust.

A: When I fetch my own stock I cannot promise that. Last time I unrolled one and found a previous guest's footprint underneath.

B: You take the `malloc` road: the goods are blank, but the dust is still on them. Here I use `calloc`, and I sweep every mat before handing it over, right into the seams.

A: And what if I say "zero mats"?

B: Zero mats means no mat. You get an empty hand, and when you hand the empty hand back I do not even write it down.

A: That sounds fair. And if I ask for four thousand and ninety-six?

B: Laid out all the same, every one clean, each in its own place, none stepping on another.

A: Good — let me read out the problem.

**In plain words**: implement `zeros(n)`: allocate room for `n` `int` values, set every slot to zero, and return the first address; return `NULL` when `n` is `0`. The caller releases the block with `free`.

**Interface**:

```c
#include <stddef.h>

int *zeros(size_t n);
```

- `n > 0`: return a pointer to `n` `int` values, each equal to `0`; return `NULL` if the allocation fails.
- `n == 0`: return `NULL`.
- Release the result with `free`; `free(NULL)` does nothing.

### Example

After `int *p = zeros(4);` the slots `p[0]` through `p[3]` are all `0`; `zeros(0)` gives `NULL`.

### Contract

- Implement only this function; do not write `main`.
- Use the standard library's dynamic allocation, not a fixed-size array in disguise.
- Every returned slot is readable and writable, and writing one slot leaves the others in the same block untouched.
- Two allocated blocks are independent: changing one does not change the other.
