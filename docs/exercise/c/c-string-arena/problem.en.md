A camp is one large patch of ground staked out in advance. Nobody who moves in asks for land of their own; they go to the camp keeper instead. The keeper remembers where the free ground begins, cuts a piece for each newcomer, and moves the line forward.

So requests to the system are rare, and giving ground back is simple: hand back the whole camp, or move the line back to the start and begin again.

**In plain words**: `arena_init` obtains `bytes` bytes once; `arena_alloc` cuts `n` bytes from `used`; `arena_strdup` puts a string there together with its terminator; `arena_reset` moves `used` back to `0`; `arena_free` hands the whole patch back.

**Interface**:

```c
typedef struct {
    char *base;
    size_t cap;
    size_t used;
} Arena;

int arena_init(Arena *a, size_t bytes);
void *arena_alloc(Arena *a, size_t n);
char *arena_strdup(Arena *a, const char *s);
void arena_reset(Arena *a);
void arena_free(Arena *a);
size_t arena_used(const Arena *a);
```

- `arena_init` returns `0` on success; it returns `-1` when `bytes` is `0` or the request fails.
- `arena_alloc` returns the start of the piece and increases `used` by `n`; when the piece does not fit or `n` is `0` it returns `NULL` and leaves `used` alone.
- Pieces sit next to each other in the order they were cut: the first starts at `base`, the second right after the first.
- `arena_strdup` takes `strlen(s) + 1` bytes and copies `s` along with its `'\0'`; it returns `NULL` when `s` is `NULL` or the string does not fit.
- `arena_reset` sets `used` to `0`; `base` and `cap` stay.
- `arena_free` hands the whole patch back, setting `base` to `NULL` and `cap` and `used` to `0`; calling it twice is harmless.
- `arena_used` returns how many bytes are in use.

### Example

```c
Arena a;
arena_init(&a, 64);              /* one patch of 64 bytes */
char *s = arena_strdup(&a, "hi");    /* 3 bytes, used == 3 */
void *p = arena_alloc(&a, 10);       /* the next 10 bytes, used == 13 */
arena_reset(&a);                     /* used back to 0, the patch stays */
arena_free(&a);                      /* the whole patch goes back */
```

### Contract

- Implement only these six functions; do not write `main`.
- `arena_init` asks the system once; `arena_alloc` and `arena_strdup` never ask it again.
- Pieces do not overlap, and a later piece never starts below an earlier one.
