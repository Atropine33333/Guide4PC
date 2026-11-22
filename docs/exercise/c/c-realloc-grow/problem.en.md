(A deep-space freight station. Beyond the porthole all is black; inside, one row of cargo slots remains.)

A: The row is empty right now, not one slot taken. The first shipment is here — how do you load it?

B: I open four slots. When all four are full, I stretch the row to eight, and the four crates stay exactly where they were, not one of them moved.

A: And when the eight are full?

B: Eight becomes sixteen, sixteen becomes thirty-two; it doubles each time. No old crate changes its place.

A: And if the station cannot spare the room, so the row cannot stretch?

B: Then I report a failure: the row, the count and the capacity all stay as they were, and we try again on the next run. Not one old crate may be lost.

A: Noted. Let me read out the problem.

**In plain words**: implement `append` to add `v` at the end of a dynamic array described by `*a` (first address), `*n` (elements in use) and `*cap` (capacity). When the array is full, grow it and keep every old element in place and in order.

**Interface**:

```c
int append(int **a, int *n, int *cap, int v);
```

- The initial state may be `*a = NULL`, `*n = 0`, `*cap = 0`, or an already allocated array holding some elements.
- `*n < *cap`: write `v` at index `*n`, increment `*n`, and return `0`.
- `*n == *cap`: the new capacity is `*cap == 0 ? 4 : *cap * 2`; after the grow succeeds, store the value, update all three values, and return `0`.
- If the grow fails: return `-1`, leave `*a`, `*n` and `*cap` untouched, and keep the old elements reachable.
- Return `0` on every success.

### Example

Start with `NULL`, `0`, `0` and append `5`, `6`, `7`, `8`: the capacity is now `4`. Append `9`: the capacity doubles to `8`, indices `0` through `3` still hold `5`, `6`, `7`, `8`, index `4` holds `9`, and `*n` is `5`.

### Contract

- Implement only this function; do not write `main`.
- Grow with `realloc`, and keep the old elements.
- The first allocation for capacity `0` is `4` slots; afterwards the capacity doubles.
- `*n` always equals the number of stored elements, and `*cap` is never smaller than `*n`.
- The caller eventually releases the block with `free`.
