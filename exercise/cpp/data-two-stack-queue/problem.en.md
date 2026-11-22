NOTICE ON THE IMPLEMENTATION OF QUEUE MINIMUM QUERIES

To all implementation units:

Structures of the queue kind have always kept the rule that first in is first out; and yet it is no uncommon thing for the queue to be asked, at any moment, what its smallest element is. For a uniform standard of implementation, the scheme is hereby notified as follows.

Article 1. Internal storage shall use `std::vector` only, and only in the manner of a stack: `push_back`, `back`, and `pop_back`; indexed access shall not be used.
Article 2. `push` places the element at the rear; element values are limited to the range `0` to `10^9`.
Article 3. `front` returns the front element and `min` the smallest value currently in the queue; when the queue is empty, both return `-1`.
Article 4. `pop` removes the front element; when the queue is empty it does nothing.
Article 5. `empty` and `size` state the current number of elements truthfully.
Article 6. Every operation shall be amortized `O(1)`; `min` shall not rescan the whole queue each time.
Article 7. Completing this class is enough; no unit shall write its own `main`.

Please proceed accordingly.

**In plain words**: implement a queue on top of two stacks and let it report its minimum in O(1). Internal storage may use only `std::vector`, and only as a stack (`push_back` / `back` / `pop_back`); `push` enqueues at the rear, `pop` dequeues from the front, `front` and `min` both return -1 when the queue is empty, and `empty` and `size` describe the element count. All operations are amortized O(1), and `min` must not rescan the queue each time. **Do not write `main`**.

```cpp
#include <vector>

class TwoStackQueue {
public:
    void push(int value);
    int front() const;
    void pop();
    bool empty() const;
    int size() const;
    int min() const;
};
```

### Rules

- Internal storage may use `std::vector` only, and only as a stack (`push_back` / `back` / `pop_back`).
- `push` places the element at the rear; element values are in `0` to `10^9`.
- `front` returns the front element; `min` returns the smallest value currently in the queue; when the queue is empty both return `-1`.
- `pop` removes the front element; when the queue is empty it does nothing.
- `empty` and `size` describe the current element count.
- Every operation must be amortized `O(1)` — `min` must not rescan the whole queue each time.
- Completing the class is enough; do not write your own `main`.

### Hint

Each of the two stacks keeps a running minimum from its bottom to its current top. When elements are poured from one stack into the other, the minima must be rebuilt along the way; the queue's minimum is the smaller of the two stacks' minima.
