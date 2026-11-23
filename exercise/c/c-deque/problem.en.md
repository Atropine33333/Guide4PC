# Both Ends

The queue at the canteen window works from both sides: someone can still squeeze in at the front, and someone else can join at the back. At serving time people leave from the front; at closing time they are cleared from the back. Your job is to manage a double-ended queue that holds up to 64 integers.

**In plain words**: implement a 64-slot double-ended queue where both the front and the back accept pushes and pops; a full deque refuses pushes and an empty one refuses pops.

**Interface**:

```c
#define DEQUE_CAP 64

typedef struct {
    int data[DEQUE_CAP];
    int head;
    int size;
} Deque;

void deque_init(Deque *d);
int deque_push_front(Deque *d, int value);
int deque_push_back(Deque *d, int value);
int deque_pop_front(Deque *d, int *out);
int deque_pop_back(Deque *d, int *out);
int deque_size(const Deque *d);
```

- `deque_push_front` and `deque_push_back` return `0` on success; with all 64 slots taken they return `-1` and leave the deque unchanged.
- `deque_pop_front` and `deque_pop_back` write the element into `*out` and return `0`; on an empty deque they return `-1` and leave `*out` alone.
- `deque_size` returns the number of elements.

### Example

After `deque_push_back(&d, 1)`, `deque_push_back(&d, 2)` and `deque_push_front(&d, 0)`, the deque reads `0 1 2` from front to back. `deque_pop_front` yields `0`, `deque_pop_back` yields `2`, and only `1` remains.

### Contract

- Implement only these functions; do not write `main`.
- No dynamic allocation; the capacity is `DEQUE_CAP`.
- After the indices wrap past `0`, the order inside the deque must stay the same.
- A failed operation must not change the structure.
