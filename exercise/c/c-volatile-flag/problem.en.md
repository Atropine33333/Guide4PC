THE LAMP THAT CAN CHANGE

Observation Post 3 has a single status lamp. It goes dark and bright without warning; nobody can promise it looks the same between two glances.

The watchkeeper's habit is plain: fix a maximum number of glances, `limit`, then look again and again. On the glance that finds the lamp lit, write down `1` and stop; if all `limit` glances find it dark, write down `0`. The lamp may be changed from elsewhere at any moment, so every glance has to look at the lamp afresh instead of trusting what the first glance saw.

**Interface**:

```c
volatile int flag;          /* the lamp, initially 0 */

void set_flag(void);
void clear_flag(void);
int wait_until_set(int limit);
```

- `set_flag` sets `flag` to `1`; `clear_flag` sets `flag` to `0`.
- `wait_until_set` takes at most `limit` glances: if any glance finds `flag` non-zero it returns `1`; if all `limit` glances find it zero it returns `0`.
- When `limit ≤ 0` it takes no glance at all and returns `0`.

**In plain words**: implement `set_flag`, `clear_flag` and `wait_until_set`. `wait_until_set(limit)` loops at most `limit` times and checks the global `flag` each time; it returns `1` when the flag is non-zero, and `0` otherwise.

### Example

```c
clear_flag();
wait_until_set(5)   // the lamp stays dark for all 5 glances: 0
set_flag();
wait_until_set(1)   // lit on the very first glance: 1
clear_flag();
wait_until_set(0)   // no glance at all: 0
```

### Contract

- Implement only these three functions; do not write `main`. The global `flag` is already defined and initialised to `0` in the starting code; keep it global.
- `flag` is `volatile`: read it again on every pass, and do not copy it into a local first.
- `limit` may be `0` or negative, in which case the loop must not run at all.
- The function must always finish; a dark lamp cannot keep it spinning forever.
