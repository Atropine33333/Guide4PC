Implement an integer buffer that owns a dynamic array.

Implement `IntBuffer`:

- `explicit IntBuffer(int size)`: allocate `size` integers, all initially `0`; `size` may be `0`.
- `IntBuffer(const IntBuffer& other)`: deep copy, so the two buffers are independent afterwards.
- `IntBuffer& operator=(const IntBuffer& other)`: deep-copy assignment that supports self-assignment and does not leak the old memory.
- `~IntBuffer()`: release the memory it owns.
- `int size() const`: return the length.
- `bool set(int index, int value)`: write and return `true` when the index is valid; return `false` when out of range.
- `bool get(int index, int& out) const`: read and return `true` when the index is valid; return `false` when out of range.
- `void fill(int value)`: set every element to `value`.

Do not use `std::vector` as the internal storage; this exercise is about managing `new[]` / `delete[]` yourself. Repeated construction, copying, and assignment must leave no double free or dangling pointer.
