Implement an integer stack that can report its current minimum at any time.

Implement `MinStack` with the following members:

- `void push(int value)`: push `value`.
- `bool pop()`: remove the top element and return `true`; return `false` without changing the stack when empty.
- `bool top(int& out) const`: if non-empty, write the top element to `out` and return `true`; otherwise return `false`.
- `bool min(int& out) const`: if non-empty, write the current minimum to `out` and return `true`; otherwise return `false`.
- `int size() const`: return the number of elements.

Every operation should be O(1). Hint: a single stored minimum is not enough — after the smallest element is popped the previous minimum must return; keep a helper stack.
