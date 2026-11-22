**EXODUS: THE CHAIN**

And the Lord spoke to Moses, saying:

"Make a chain for the people. Let every link face forward, and let it look back only once. The one that looks back is counted as seeing, and not as owning."

And Moses said, "What if a man looks back and puts out his hand to hold?"

And the Lord said, "He shall not. The chain that reaches back becomes a ring, and no link in the ring will let go, and the last link shall not go out. Make the backward cord a weak cord: it sees, and it does not take."

"The length of the chain is given by `length`, and the links are numbered from `0` upward. The first link looks back at nothing; the last link looks forward at nothing."

"Whoever counts the links walks the forward cord, one link, one count, and stops at the empty place. Whoever examines the chain shall see that every link looks back upon the one before it, and that the first link's backward look has already failed."

"And when the people leave the camp, they shall not make a `main` for themselves."

**In plain words**: implement three functions. `build_chain(length)` builds a doubly linked chain of exactly `length` nodes with `value`s from `0` to `length - 1`; `next` is a `shared_ptr`, and `previous` must be a `weak_ptr` that adds no ownership; `length <= 0` returns a null pointer. `chain_length(head)` counts nodes along `next`, returning 0 for a null pointer. `links_consistent(head)` checks that for every non-empty `next`, the successor's `previous.lock()` points back to that node, and that the head's `previous` has already expired; a null pointer is consistent. Do not write `main`.

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

### Example

| Call | Result |
| --- | --- |
| `build_chain(0)` | a null pointer |
| `chain_length(build_chain(5))` | `5` |
| `links_consistent(build_chain(5))` | `true` |

### Hint

While building, assign `node->previous = current;` to store a `shared_ptr` into a `weak_ptr`. The tail's `previous` is weak, so the whole chain is owned only by the head's `shared_ptr`; once the head is released, `previous.lock()` on the tail yields null. Compare addresses with `.get()` in `links_consistent`.
