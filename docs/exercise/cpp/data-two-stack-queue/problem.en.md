This exercise reads no stdin. Implement a first-in-first-out queue using two stacks.

```cpp
#include <vector>

class TwoStackQueue {
public:
    void push(int value);
    int front() const;
    void pop();
    bool empty() const;
    int size() const;
};
```

Rules:

- The implementation may only use `std::vector` as its internal storage, treating it as a stack (`push_back` / `back` / `pop_back`).
- `push` appends to the back of the queue.
- `front` returns the element at the front, or `-1` when the queue is empty.
- `pop` removes the front element; it does nothing when the queue is empty.
- `empty` and `size` describe the current number of elements.
- Do not write `main`.

### Hint

Keep two stacks: `in` receives new elements and `out` serves the front. Only when `pop` finds `out` empty do you pour all of `in` into `out`. `front` can look at the bottom of `in` with `in.front()` without moving anything.
