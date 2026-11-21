This one reads no stdin and writes no stdout. You implement a class; the test logic lives in a hidden harness that is compiled after your code and calls `push`, `pop` and `size` directly.

Implement `CircularBuffer` with a fixed capacity set in the constructor:

- `push(value)`: write and return `true` if not full; return `false` if full;
- `pop(out)`: move the oldest value into `out` and return `true` if not empty; return `false` if empty;
- `size()` / `capacity()`: current element count / total capacity.

Do not write a `main`; the harness provides the entry point. The starter is a compilable placeholder: every check fails until you add the members and fill in the methods.
