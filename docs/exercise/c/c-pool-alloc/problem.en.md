(The manifest of a cargo ship. The whole ship has one row of cells, numbered 0 to 7, each exactly 32 bytes.)

The captain says: never mind what the cargo is. The cells are this big, and anything that does not fit stays ashore.

Loading starts at the lowest-numbered free cell and works upward. Once all eight are out, the next request only gets "nothing left".

Unloading is strict: only the start address of a cell counts. Return the same cell twice and the second return is refused; hand over an address that is not on the manifest and it is refused too.

In port the hold is cleared in one go, all eight cells count as free again, and the next run starts from the bottom.

**In plain words**: implement a fixed-size block pool. `pool_init` empties every cell; `pool_alloc` hands out the lowest-numbered free cell; `pool_free` accepts only the start address of a cell that is currently out; `pool_reset` takes every cell back at once.

**Interface**:

```c
#define POOL_BLOCK_SIZE 32
#define POOL_BLOCK_COUNT 8

typedef struct {
    unsigned char cells[POOL_BLOCK_COUNT][POOL_BLOCK_SIZE];
    unsigned char used[POOL_BLOCK_COUNT];
    int in_use;
} Pool;

void pool_init(Pool *p);
void *pool_alloc(Pool *p);
int pool_free(Pool *p, void *block);
void pool_reset(Pool *p);
int pool_in_use(const Pool *p);
```

- `pool_init` clears every `used` flag and sets `in_use` to `0`.
- `pool_alloc` returns the start address of the lowest-numbered free cell and marks it taken; it returns `NULL` when no cell is free or `p` is `NULL`.
- `pool_free` returns `0` when `block` is exactly the start address of a cell that is currently out, and takes that cell back.
- When `block` is `NULL`, is not a cell start, or the cell is already free, `pool_free` returns `-1` and leaves the pool unchanged.
- `pool_reset` takes every cell back and sets `in_use` to `0`; `pool_reset(NULL)` does nothing.
- `pool_in_use` returns how many cells are out.

### Example

```c
Pool p;
pool_init(&p);
void *a = pool_alloc(&p);        /* &p.cells[0] */
void *b = pool_alloc(&p);        /* &p.cells[1] */
pool_free(&p, a);                /* 0 */
pool_free(&p, a);                /* -1, already free */
void *c = pool_alloc(&p);        /* &p.cells[0] again */
pool_reset(&p);                  /* all eight free */
```

### Contract

- Implement only these five functions; do not write `main`.
- The cells are the arrays inside `Pool`; obtain no extra memory.
- The caller fills a cell; `pool_free` does not clear its bytes.
- After a cell is returned, the same cell may be handed out again.
