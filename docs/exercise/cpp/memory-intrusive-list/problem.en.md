Now then. On this day a company of men drew up on the parade ground, and their banner read `IntrusiveList`. Honoured listeners, this company has a strange temper: it knows every man and feeds none of them.

How so? The nodes `Node` are brought by the caller; the list merely ties the two straps `prev` and `next` so that each man knows the one before and behind. Lodging and wages are nobody's business, and when the company disbands it may not `delete` a single man.

First the roll call: `push_back` ties a newcomer to the tail. He must be alive, and must not already be tied into another company. `unlink` pulls a man out and wipes his `prev` and `next` to `nullptr`. If he never belonged to this company, or never came at all, nothing happens.

`head` and `tail` report the front and rear, or `nullptr` when the company is empty; `empty` and `size` report the count. At disbanding the company disperses and draws no blade.

Someone asks: where do these nodes sit, and will they move? Rest easy. The harness has already led the nodes into a `std::vector<Node>` with its capacity reserved up front, so their addresses stand like mountains while you tie the straps.

**In plain words**: implement `IntrusiveList`, which maintains only the `prev` / `next` links: it never allocates or frees a node, and its destructor must not `delete` anything. `push_back` appends a node; the caller guarantees the node is non-null and belongs to no list. `unlink` removes a node and sets its `prev` and `next` to `nullptr`, and does nothing when the node is `nullptr` or not in this list. `head` / `tail` return `nullptr` for an empty list, and `size` / `empty` describe the count. This exercise reads no stdin; do not write `main`.

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
