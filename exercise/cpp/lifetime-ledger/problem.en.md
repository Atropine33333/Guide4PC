This exercise reads no stdin and writes no stdout. Implement a class that tracks how many live objects exist; a hidden harness runs the tests.

Implement `Tracked`:

- `Tracked()`: constructs with value `0`.
- `explicit Tracked(int value)`: constructs with value `value`.
- `Tracked(const Tracked& other)`: copy construction copies the value; this is a **new object**.
- `Tracked& operator=(const Tracked& other)`: assignment copies the value; the object count does not change, and self-assignment is safe.
- `~Tracked()`: destructor.
- `int value() const` and `void setValue(int value)`: read and write the value.
- `static int alive()`: return the number of live objects.

The alive count starts at `0`. Construction and copy construction increment it, destruction decrements it, and assignment leaves it unchanged. The harness creates objects in nested scopes, copies them, stores them in arrays, and checks that the count returns to zero when each scope ends.
