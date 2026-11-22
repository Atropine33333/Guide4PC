This exercise reads no stdin and writes no stdout. Build a doubly linked chain with `shared_ptr` and `weak_ptr` that never forms an ownership cycle, tested by a hidden harness.

```cpp
#include <memory>

struct Node {
    int value = 0;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> previous;
};

std::shared_ptr<Node> build_chain(int length);
std::size_t chain_length(const std::shared_ptr<Node>& head);
bool links_consistent(const std::shared_ptr<Node>& head);
```

Rules:

- `build_chain(length)` creates a chain of exactly `length` nodes whose `value`s are `0, 1, ..., length - 1`.
- Each node's `next` points to the following node; the last node's `next` is empty.
- Each node's `previous` points to the preceding node, and the first node's `previous` is empty; `previous` must be a weak reference and must not add ownership.
- `length <= 0` returns a null pointer.
- `chain_length(head)` counts nodes following `next`; a null pointer returns 0.
- `links_consistent(head)` checks the links: for every node with a non-empty `next`, the successor's `previous.lock()` must point back to that node, and the head's `previous` must already be expired. A null pointer is consistent.
- Do not write `main`.

### Example

| Call | Result |
| --- | --- |
| `build_chain(0)` | a null pointer |
| `chain_length(build_chain(5))` | `5` |
| `links_consistent(build_chain(5))` | `true` |

### Hint

While building, assign `node->previous = current;` to store a `shared_ptr` into a `weak_ptr`. The tail's `previous` is weak, so the whole chain is owned only by the head's `shared_ptr`; once the head is released, `previous.lock()` on the tail yields null. Compare addresses with `.get()` in `links_consistent`.
