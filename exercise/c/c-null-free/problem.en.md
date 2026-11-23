Give a pointer slot a tidy way to give its memory back: when the slot holds an address, return the memory it points to and set the slot to empty; when the slot is already empty, or the slot itself does not exist, do nothing at all.

**In plain words**: implement `safe_free`. The argument is the address of a pointer variable; if `p` is `NULL` or `*p` is `NULL`, return at once, otherwise free the memory that `*p` points to and set `*p` to `NULL`.

**Interface**:

```c
#include <stddef.h>

void safe_free(void **p);
```

- `p == NULL`: do nothing.
- `*p == NULL`: do nothing.
- Otherwise: free the memory that `*p` points to, then set `*p` to `NULL`.
- Passing a slot that was already freed again shows `NULL`, so the call does nothing.
- Two slots are independent; freeing one does not affect the other.

### Example

```c
void *p = malloc(32);
safe_free(&p);    /* p becomes NULL */
safe_free(&p);    /* does nothing */
safe_free(NULL);  /* does nothing */
```

### Contract

- Implement only this function; do not write `main`.
- Free only memory obtained from facilities such as `malloc`, `calloc`, `realloc`.
- Handle only the slot passed in; do not hunt for the same address stored elsewhere.
- After freeing, the old address is no longer used.
