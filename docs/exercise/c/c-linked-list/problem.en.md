A singly linked list starts at `head`; each node carries an `int` and a pointer to the next node, and the last node's `next` is `NULL`. Your job is to run this list: put items at the front, take them from the front, report the length, test for emptiness, and finally tear the whole thing down.

**Interface**:

```c
#include <stddef.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    size_t size;
} IntList;

void list_init(IntList *l);
int list_push_front(IntList *l, int value);
int list_pop_front(IntList *l, int *out);
size_t list_size(const IntList *l);
int list_is_empty(const IntList *l);
void list_free(IntList *l);
```

- `list_init` sets `head` to `NULL` and `size` to `0`.
- `list_push_front` allocates a new node with `malloc` and links it at the front; it returns `0` on success and `-1` when allocation fails, leaving the list unchanged.
- `list_pop_front` unlinks the front node, writes its value into `*out`, frees the node and returns `0`; on an empty list it returns `-1` and leaves `*out` alone.
- `list_size` returns the number of nodes; `list_is_empty` returns `1` for an empty list and `0` otherwise.
- `list_free` releases every node and resets `head` to `NULL` and `size` to `0`, so the list can be used again afterwards.

### Example

Call `list_push_front` with `1`, `2`, `3` in that order; the list reads `3 2 1` from front to back. Pop once and get `3`; pop again and get `2`, with `list_size` now `1`. After `list_free`, `list_size` is `0` and `list_is_empty` returns `1`.

### Contract

- Implement only these functions; do not write `main`.
- Nodes come from `malloc`; there is no fixed length limit, and operations on an empty list must not crash.
- Keep the struct fields as given and the function signatures unchanged.
