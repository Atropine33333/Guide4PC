Now hear this tale of a singly linked list, where each node holds fast to the next: `value` is the cargo, `next` is the rope. Gentle listeners, the list begins empty, not one node to its name; `push_front` merely hangs a node on the head, and that hanging is O(1).

And so came the day for the trick called "reversal in place". The management had long since laid down the order: no second list may be built to hold the cargo, and the goods may not merely be swapped two by two — the direction of the ropes themselves must truly turn.

See the performer now, three fingers at the ready: one on the one before, one on the present, one on the one after. He first notes the far rope in his palm, then ties the present rope back to the one before, and then all three fingers shift one step along. At the end of the rope, whatever the first finger holds is the new head.

Gentle listeners, by that road a whole list turns about, and `size_` gains not one and loses not one. And when the curtain falls, the destructor must gather every node, and gather an empty list cleanly too, or the performance is not complete.

**In plain words**: complete the singly linked list class `IntList`. `push_front` inserts a value at the head in O(1); `reverse` reverses in place, without building a second list and without merely swapping values — keep previous, current, and next pointers; `head()` returns the node chain from the head to `next == nullptr`, and `nullptr` for an empty list; `size()` returns the node count, unchanged by reversal and updated on insertion; the destructor frees every node and is safe on an empty list. The copy constructor and copy assignment are declared `= delete`; do not implement them. Do not write `main`, and do not change the public interface.

```cpp
struct Node {
    int value;
    Node* next;
};

class IntList {
public:
    IntList();
    ~IntList();
    IntList(const IntList&) = delete;
    IntList& operator=(const IntList&) = delete;

    void push_front(int value);
    void reverse();
    int size() const;
    const Node* head() const;

private:
    Node* head_;
    int size_;
};
```

Rules:

- A new list is empty: `head()` returns `nullptr` and `size()` returns 0.
- `push_front` inserts a value at the head in O(1).
- `reverse` reverses the list in place: do not build a second list and do not merely swap values.
- The destructor frees every node and must be safe on an empty list.
- The copy constructor and copy assignment are declared `= delete`; do not implement them.
- `head()` returns the node chain from the head, ending with `next == nullptr`.
- Do not write `main`, and do not change the public interface above.

### Hint

During reversal keep three pointers: previous, current, and next. `size_` does not change when reversing, but it must change on insertion.
