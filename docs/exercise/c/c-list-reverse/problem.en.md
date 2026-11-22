# Calling Backwards

(A small stage. Two stools; A and B each take one.)

A: Today's tale is about a school. The master always called the roll from the front. One day he says: call it backwards.

B: Then start from the back of the line.

A: The trouble is, each person in line remembers only who stands behind him; who stands ahead, nobody knows.

B: Ah, so the line must be rebuilt.

A: The master walks from the front. At each person, he has him rest a hand on the shoulder of the one just before.

B: One by one, all the way down?

A: At the end of the line, the last person becomes the head; the old head has nobody behind him and becomes the tail.

B: Same people, only the facing changed.

A: What about an empty line?

B: Nobody to call; it stays empty.

A: And a line of one?

B: He turns around and is still himself.

**In plain words**: reverse the links of a singly linked list: the old head becomes the tail and the old tail becomes the head, with no node created or destroyed.

**Interface**:

```c
typedef struct Node {
    int value;
    struct Node *next;
} Node;

Node *list_reverse(Node *head);
```

`head` is the first node; return the new head. An empty list returns `NULL`.

### Example

`1 → 2 → 3` becomes `3 → 2 → 1`.

### Contract

- Implement only `list_reverse`; do not write `main`.
- Change only the `next` pointers; create no node and change no `value`.
- The list has no cycle.
