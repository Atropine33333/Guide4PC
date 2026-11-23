# Rules of the Camp

A wooden board is nailed beside the camp gate. Rain has darkened its edges, but six lines can still be read.

1. This camp has one patch of ground, called the camp, 256 bytes in all. Take stock on entry and record the used amount as 0.
2. Items are laid out from the head of the ground onwards. The space each item takes is rounded up to 8 bytes, and every starting edge falls on a multiple of 8.
3. If the ground cannot hold an item, refuse on the spot. On a refusal the used amount does not change, and nothing already handed out moves by a hair.
4. A request for 0 items hands out nothing, and the used amount does not change either.
5. Striking camp clears it: the used amount returns to 0 and old marks on the ground are not swept away. The next group still starts from the head of the ground, and the first item begins exactly where the previous group's first item began.
6. The used amount must be reportable at any time.

**In plain words**: implement a fixed-size arena: `arena_init` takes stock, `arena_alloc` cuts a block from the used mark upwards with 8-byte rounding, `arena_reset` sets the used amount back to 0, and `arena_used` reports the current number of used bytes.

**Interface**:

```c
#include <stddef.h>

#define ARENA_CAP 256

typedef struct {
    _Alignas(8) unsigned char buf[ARENA_CAP];
    size_t used;
} Arena;

void arena_init(Arena *a);
void *arena_alloc(Arena *a, size_t n);
void arena_reset(Arena *a);
size_t arena_used(const Arena *a);
```

- `arena_init`: set `used` to 0.
- `arena_alloc(a, n)`: return `NULL` when `n == 0`; return `NULL` when `n > ARENA_CAP`; the request needs `((n + 7) / 8) * 8` bytes; if `used + need > ARENA_CAP`, return `NULL` and leave `used` unchanged; otherwise return `a->buf + used` and add `need` to `used`.
- The returned address is a multiple of 8, and `n` readable and writable bytes follow it.
- `arena_reset`: set `used` to 0. Old bytes are not cleared, and the next item still starts at `a->buf`.
- `arena_used`: return the current number of used bytes.
- There is no separate return of an item; the whole patch is cleared at once.

### Example

```c
Arena a;
arena_init(&a);                 /* used is 0 */
void *p = arena_alloc(&a, 16);  /* p == a.buf, used becomes 16 */
void *q = arena_alloc(&a, 20);  /* q == a.buf + 16, 20 rounds to 24, used becomes 40 */
arena_reset(&a);                /* used returns to 0 */
void *r = arena_alloc(&a, 16);  /* r == p, laid out from the head again */
```

### Contract

- Implement only these four functions; do not write `main`.
- The camp is the `buf` inside the struct; do not obtain dynamic memory elsewhere.
- `used` accumulates rounded byte counts, so it is always a multiple of 8.
- A refused request changes neither `used` nor existing data.
- Every block handed out is readable and writable, and writing one block does not change another.
- An out-of-range request damages nothing.
