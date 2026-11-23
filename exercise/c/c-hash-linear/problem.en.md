A cabinet has 16 slots numbered `0` to `15`. An integer first goes to slot `(unsigned)key % 16`; if that slot is taken, it moves one slot along, and after the last slot it comes back to `0`. Lookups and removals follow the same path. A removed slot keeps an "erased" mark so that elements sitting behind it can still be found.

**Interface**:

```c
#include <stddef.h>

#define HASH_CAP 16

typedef struct {
    int keys[HASH_CAP];
    unsigned char state[HASH_CAP]; /* 0 empty, 1 occupied, 2 erased */
    size_t count;
} HashSet;

void hash_init(HashSet *s);
int hash_set(HashSet *s, int key);
int hash_get(const HashSet *s, int key);
int hash_remove(HashSet *s, int key);
size_t hash_size(const HashSet *s);
```

- `hash_init` clears every slot and resets the count.
- `hash_set` probes linearly from `(unsigned)key % HASH_CAP`: if the key is already stored it returns `0` without taking a second slot; at an empty or erased slot it stores the key and returns `0`; if all 16 slots have been tried and none works, it returns `-1`.
- `hash_get` returns `1` when found; it returns `0` upon reaching an empty slot or after a full loop.
- `hash_remove` marks the slot erased and returns `0` when found, or returns `-1` when it is absent. A removal decreases `hash_size` by one.
- `hash_size` returns the number of stored elements. Negative keys use the same formula through the `(unsigned)` conversion.

### Example

Insert `1`, `17`, `-15`: all three start from slot 1 and end up in slots 1, 2 and 3. Now `hash_get(17)` is `1` and `hash_size` is `3`. After removing `17`, `hash_get(1)` and `hash_get(-15)` still return `1`, `hash_get(17)` returns `0`, and inserting `49` takes the slot that was just freed.

### Contract

- Implement only these functions; do not write `main`.
- The table stays at `HASH_CAP` slots; do not grow it dynamically.
- The slot formula is fixed at `(unsigned)key % HASH_CAP`, with linear probing from there.
- Keep the struct fields as given and the function signatures unchanged.
