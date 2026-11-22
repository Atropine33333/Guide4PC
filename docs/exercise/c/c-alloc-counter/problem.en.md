Give the allocator a ledger: one entry whenever a block goes out, another whenever a block comes back. The ledger holds two numbers, total borrows and total returns; their difference is the number of blocks still out. Keep the books honest: a failed request is not an entry, and a null pointer is not an entry.

**In plain words**: wrap `malloc` and `free`. `ac_alloc` adds one to `allocs` only when the request succeeds; `ac_free` adds one to `frees` only when it receives a non-null pointer; `ac_outstanding` returns the difference.

**Interface**:

```c
typedef struct {
    size_t allocs;
    size_t frees;
} AllocStats;

void ac_init(AllocStats *st);
void *ac_alloc(AllocStats *st, size_t size);
void ac_free(AllocStats *st, void *p);
long ac_outstanding(const AllocStats *st);
```

- `ac_init` sets both `allocs` and `frees` to `0`; earlier numbers count for nothing.
- `ac_alloc` requests `size` bytes; on success it returns the pointer and adds one to `allocs`.
- When `size` is `0`, the request fails, or `st` is `NULL`, it returns `NULL` and adds nothing.
- `ac_free` returns `p`; when `p` is `NULL` it does nothing and `frees` stays put.
- `ac_outstanding` returns `allocs - frees`, never below `0`.

### Example

```c
AllocStats st;
ac_init(&st);
void *a = ac_alloc(&st, 10);   /* allocs = 1 */
void *b = ac_alloc(&st, 20);   /* allocs = 2 */
ac_free(&st, a);               /* frees = 1 */
long left = ac_outstanding(&st);   /* 1 */
ac_free(&st, b);                   /* 0 blocks left */
```

### Contract

- Implement only these four functions; do not write `main`.
- Use only the standard memory functions; both counters in the ledger are `size_t`.
- Return only pointers you handed out; `ac_free` does not check where a pointer came from.
