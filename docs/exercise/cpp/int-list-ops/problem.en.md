This exercise reads no stdin and writes no stdout. Implement a singly linked integer list; a hidden harness runs the tests.

Implement `IntList` with the following members:

- `IntList()`: constructs an empty list.
- `~IntList()`: frees every node the list owns; nothing may leak.
- `void pushFront(int value)`: insert a value at the front.
- `bool popFront()`: remove the front node and return `true`; return `false` and do nothing when the list is empty.
- `void reverse()`: reverse the list in place without allocating new nodes.
- `bool get(int index, int& out) const`: `index` is 0-based; if valid, write the value to `out` and return `true`, otherwise return `false`.
- `int size() const`: return the number of elements.

Do not use `std::list` or `std::vector` as the internal storage; this exercise is about managing node pointers yourself. The harness creates and destroys several `IntList` objects, so the destructor must release the memory correctly.
