(A small stage. One round table, eight place settings.)

A: Tonight, a shop. Eight place settings, no more. A guest arrives, a setting goes down, until the table is full.

B: First come, first served.

A: And when it is full and another guest arrives?

B: The door says "full" — not one more setting goes down.

A: And when a guest leaves?

B: A setting comes off, and the next guest takes that place.

A: I notice your hand: after the eighth setting, the next one goes back to the first.

B: Right. The eight settings form a ring; place the ninth and you are back at the first. That is the wrap.

A: How do you keep track? You surely do not count from the start each time.

B: Two numbers suffice: where the first setting sits, and how many are down. To add one, move the tail forward a slot, wrapping to zero at the end; to remove one, move the head forward a slot, wrapping the same way. Full means no more adding; empty means no more removing.

A: And when it is empty?

B: Say "empty" and leave everything as it stands.

A: Good — let me read out the problem.

**In plain words**: implement a ring buffer with capacity 8: `ring_push` writes at the tail, `ring_pop` takes from the head, and both return `-1` when the buffer is full or empty.

**Interface**:

```c
#define RING_CAP 8

typedef struct { int data[RING_CAP]; int head; int size; } RingBuf;

void ring_init(RingBuf *r);
int ring_push(RingBuf *r, int value);
int ring_pop(RingBuf *r, int *out);
int ring_peek(const RingBuf *r, int *out);
int ring_size(const RingBuf *r);
```

- `ring_init` sets the buffer to empty.
- `ring_push` writes `value` at the tail and returns `0`; when all 8 slots are taken it returns `-1` and leaves the buffer unchanged.
- `ring_pop` writes the head element into `*out`, removes it, and returns `0`; on an empty buffer it returns `-1` and leaves `*out` alone.
- `ring_peek` writes the head element into `*out` without removing it and returns `0`; on an empty buffer it returns `-1` and leaves `*out` alone.
- `ring_size` returns the number of elements, between `0` and `8`.

### Example

Write `1`, `2`, `3` in turn; the head is `1`. Pop once and get `1`; pop again and get `2`; pop again and get `3`. The buffer is now empty, so another pop returns `-1`. After writing eight values, another write returns `-1`.

### Contract

- The capacity is fixed by `RING_CAP` at 8; no dynamic allocation.
- Once all 8 slots are taken, a further write returns `-1` and the eight stored elements keep their order.
- Wrap: when the tail passes the end of `data`, it continues at index `0`; the head wraps the same way.
- Implement only these functions; do not write `main`.
