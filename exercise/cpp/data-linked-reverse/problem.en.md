This exercise reads no stdin and writes no stdout. Complete a singly linked list class; a hidden harness runs the tests.

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
