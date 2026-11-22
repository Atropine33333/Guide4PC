A row of windows, eight places, first come first served. People leave from the front and join at the back; the front marker keeps moving forward and, after the eighth slot, wraps back to the first and the line continues like a turntable.

**Interface**:

```c
#include <stddef.h>

#define QUEUE_CAP 8

typedef struct {
    int data[QUEUE_CAP];
    size_t head;
    size_t len;
} IntQueue;

void queue_init(IntQueue *q);
int queue_push(IntQueue *q, int value);
int queue_pop(IntQueue *q, int *out);
size_t queue_size(const IntQueue *q);
int queue_is_empty(const IntQueue *q);
int queue_is_full(const IntQueue *q);
```

- `queue_init` sets the front index and the length to `0`.
- `queue_push` puts `value` at the back; a full queue returns `-1` and stays unchanged.
- `queue_pop` writes the front element into `*out` and returns `0`; an empty queue returns `-1` and leaves `*out` alone.
- `queue_size` returns the number of elements; `queue_is_empty` returns `1` when empty; `queue_is_full` returns `1` when full.
- There are only `QUEUE_CAP` slots, so the back index has to wrap around to `0`.

### Example

The capacity is 8. After pushing `1` through `8` the queue is full; pop `1` through `5`, then push `9`, `10`, `11`, and the queue holds `6 7 8 9 10 11`. Six pops return exactly `6 7 8 9 10 11`, and the next pop returns `-1`.

### Contract

- Implement only these functions; do not write `main`.
- No dynamic allocation; the capacity is `QUEUE_CAP`.
- First in, first out must hold no matter where the front has wrapped to.
- Keep the struct fields as given and the function signatures unchanged.
