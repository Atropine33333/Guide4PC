This exercise reads no stdin and writes no stdout. Implement a generic stack whose capacity is fixed at compile time; a hidden harness runs the tests.

Implement the class template `BoundedStack<T, Capacity>`, where `Capacity` is a positive integer:

- `bool push(const T& value)`: push and return `true` when not full; return `false` and leave the stack unchanged when full.
- `bool pop()`: remove the top element and return `true` when non-empty; return `false` when empty.
- `bool top(T& out) const`: if non-empty, write the top element to `out` and return `true`; otherwise return `false`.
- `int size() const`: return the number of elements.
- `static constexpr int capacity()`: return the template parameter `Capacity`.

The harness instantiates `BoundedStack<int, 3>`, `BoundedStack<std::string, 2>`, `BoundedStack<double, 1>`, and others; the element type must be default-constructible, copy-constructible, and assignable.
