This exercise reads no stdin. Implement a forwarding function and observe that lvalues are copied while rvalues are moved.

```cpp
#include <utility>

struct Tracked {
    static int copies;
    static int moves;
    int value;

    explicit Tracked(int value);
    Tracked(const Tracked& other);
    Tracked(Tracked&& other) noexcept;
    Tracked& operator=(const Tracked& other);
    Tracked& operator=(Tracked&& other) noexcept;
    ~Tracked() = default;
};

int Tracked::copies = 0;
int Tracked::moves = 0;

template <class T>
Tracked forward_to_tracked(T&& value);
```

Rules:

- `Tracked`'s copy constructor performs `++copies`; its move constructor performs `++moves`; assignment operators do not change the counters.
- After a move, the source object's `value` becomes `0`.
- `forward_to_tracked` must perfectly forward its argument to `Tracked`'s constructor: a `Tracked` lvalue causes exactly one copy, a `Tracked` rvalue causes exactly one move, and an `int` (or another type convertible to `int`) constructs directly without increasing either counter.
- Return `Tracked(std::forward<T>(value));` directly rather than storing a local and returning it, so the return copy/move is elided.
- Do not write `main` (the static definitions of `Tracked::copies` and `Tracked::moves` are already given; do not repeat them).
