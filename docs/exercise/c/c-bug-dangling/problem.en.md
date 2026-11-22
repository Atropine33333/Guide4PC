At berth seven of the starport there is a shadow depot. Eight shadow berths, one name each, no longer than thirty-one characters. A traveller leaves a name before boarding and the shadow moves in; only when the traveller comes back for it does the berth fall empty again. The night keeper copies the name into a berth, and that berth stays taken until the owner returns.

The previous keeper copied the name and handed the berth straight back. The name stays in the berth, but the next traveller to register moves into that same berth. The slip of paper in your hand now points at someone else's shadow.

**In plain words**: implement the eight-berth pool. `pool_register` copies a name into the first free berth, keeps that berth taken, and returns the address inside the pool; `pool_release` hands a berth back. In `starter.c`, `pool_register` hands the berth back right after copying, so the returned address is overwritten by the next registration. Fix the moment of release.

**Interface**:

```c
#define POOL_CAP 8
#define SLOT_LEN 32

typedef struct {
    char slots[POOL_CAP][SLOT_LEN];
    unsigned char used[POOL_CAP];
} NamePool;

void pool_init(NamePool *p);
const char *pool_register(NamePool *p, const char *name);
void pool_release(NamePool *p, const char *slot);
```

- `pool_init` marks all eight berths free.
- `pool_register` takes the first free berth, copies `name` into it (at most 31 characters, then `'\0'`), keeps the berth taken, and returns the address inside it. With no free berth it returns `NULL`.
- `pool_release` hands back the berth that `slot` points into; only a berth start counts, any other address is ignored.
- After a release the characters stay where they are, but the berth may be overwritten by the next registration.

### Example

Register `Kepler`, then register `Void`. Each takes its own berth, and the first address still reads `Kepler`; only after `pool_release` hands the first berth back can a new registration move into it.

### Contract

- These functions are called directly; do not write `main`.
- No dynamic allocation; the berths are the array inside the struct.
- A name longer than 31 characters is cut to its first 31.
- When all eight berths are taken, return `NULL` and leave the stored names untouched.
- Berths can be registered and released repeatedly; releasing two makes room for two more.
