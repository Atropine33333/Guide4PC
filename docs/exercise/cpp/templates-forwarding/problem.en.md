Now hear this. On a certain day three guests came to the gate of `forward_to_tracked`.

The first was an lvalue, surname `Tracked`, carrying a chest of goods. The gatekeeper took the card, and by the rule copied the goods out fair and whole, leaving the chest with its owner — that trick is called a copy, and `copies` goes up by one.

The second was an rvalue, also surnamed `Tracked`. His goods need no copying; chest and all were carried into the back room — that trick is called a move, and `moves` goes up by one. When the carrying was done, the `value` in the old chest was set to `0`, and the chest stood empty.

The third was an `int`, with light baggage. He walked in and a `Tracked` was built on the spot, with no copying and no carrying, and both counters stood stock still.

Ladies and gentlemen, the whole secret lies in `T&&` and `std::forward<T>`. Every parameter is received as an rvalue reference, yet at the moment of forwarding its true rank must be restored: an lvalue remains an lvalue, an rvalue remains an rvalue. Fail that, and an lvalue is carried off as an rvalue, and the accounts are ruined.

One more thing: `return Tracked(std::forward<T>(value));` must be done in one breath. Do not build a local first and fetch it afterwards; the compiler hesitates, counts one extra copy, and the books take another entry.

**In plain words**: implement the function template `forward_to_tracked` so that it perfectly forwards its argument to `Tracked`'s constructor: an lvalue copies once, an rvalue moves once, and an `int` (or similar) constructs directly without touching either counter. The definition of `Tracked` is given.

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
