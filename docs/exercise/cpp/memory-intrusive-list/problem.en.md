This exercise reads no stdin. Implement a doubly linked list that does **not** own its nodes; the caller allocates them.

```cpp
#include <cstddef>

struct Node {
    int value;
    Node* prev;
    Node* next;
};

class IntrusiveList {
public:
    IntrusiveList();
    ~IntrusiveList();
    void push_back(Node* node);
    void unlink(Node* node);
    Node* head() const;
    Node* tail() const;
    std::size_t size() const;
    bool empty() const;
};
```

Rules:

- The list only maintains `prev` / `next` links between nodes; it neither allocates nor frees nodes, and its destructor must not `delete` anything.
- `push_back` appends a node; the caller guarantees that `node` is non-null and not currently in any list.
- `unlink` removes a node and sets its `prev` and `next` to `nullptr`; if the node is not in this list (or is `nullptr`), do nothing.
- `head` / `tail` return the first/last node, or `nullptr` for an empty list.
- `empty` and `size` describe the current node count.
- Do not write `main`.

The harness stores its nodes in a `std::vector<Node>` whose capacity is reserved up front, so node addresses stay stable during the test.
