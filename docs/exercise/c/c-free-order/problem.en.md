(A stall on the corner, a loudspeaker on loop.)

Closing time! One job tonight: take down every sign we hung up today, one at a time.

The signs went up like this: the newest goes at the front, each one pressing down on the ones before it.

Taking them down has an order too: newest first. Lift one, note where the next one hangs, then lift that one. Eight o'clock, nothing left.

**In plain words**: borrow nodes with `node_alloc` and push them onto the front of the list; `list_free_all` starts at the head, saves the address of the next node first, then hands the current node back with `node_free`, so each node is returned exactly once.

**Interface**:

```c
typedef struct Node {
    int value;
    struct Node *next;
} Node;

Node *node_alloc(void);   /* provided: borrow one node, NULL when none is left */
void node_free(Node *p);  /* provided: hand one node back */

Node *list_push_front(Node *head, int value);
size_t list_length(const Node *head);
size_t list_to_array(const Node *head, int *out, size_t cap);
void list_free_all(Node *head);
```

- `list_push_front` borrows a node, stores `value`, links it in front of the head, and returns the new head; if no node can be borrowed it returns `head` unchanged.
- `list_length` returns the number of nodes, `0` for an empty list.
- `list_to_array` writes at most `cap` values from the head into `out` and returns how many it wrote; it returns `0` for a `NULL` head.
- `list_free_all` returns the nodes one by one starting at the head; for a `NULL` head it does nothing.

### Example

```c
Node *head = NULL;
head = list_push_front(head, 1);   /* 1 */
head = list_push_front(head, 2);   /* 2 -> 1 */
head = list_push_front(head, 3);   /* 3 -> 2 -> 1 */
int out[3];
list_to_array(head, out, 3);       /* out is 3, 2, 1 */
list_free_all(head);               /* 3 first, then 2, then 1 */
```

### Contract

- Implement only the four list functions; do not write `main`.
- Take and return node memory only through `node_alloc` and `node_free`; do not call `malloc` or `free` directly.
- Save the address of the next node before returning the current one; never touch a node after returning it.
- One close-down returns every node that was on the list, each exactly once.
