Guide4PC Exercise Administration Office

Circular No. G4-TPL-001

On the Administration of Bounded Generic Stacks

To all test units:

The following provisions govern the pushing, popping, and capacity auditing of bounded stacks.

1. A stack's capacity is fixed at compile time by the template parameter `Capacity`. Once fixed, it shall not be enlarged or rebuilt.

2. A push shall first check the remaining room. If the stack is not full, admit the value and return `true`; if it is full, refuse it, return `false`, and leave the contents untouched.

3. A pop or a read of the top shall first check the stored count. If the stack is not empty, process it and return `true`; if it is empty, decline, return `false`, and do not write to `out`.

4. `size()` and `capacity()` shall report the stored count and the fixed capacity truthfully.

5. The element type must be default-constructible, copy-constructible, and assignable. What is pushed is an element, and what is handed back is an element.

Carry out accordingly.

**In plain words**: implement the class template `BoundedStack<T, Capacity>` (`Capacity` is a positive integer) with the members and behaviour below; handle capacity and empty stacks as the five provisions say.

- `bool push(const T& value)`: push and return `true` when not full; return `false` and leave the stack unchanged when full.
- `bool pop()`: remove the top element and return `true` when non-empty; return `false` when empty.
- `bool top(T& out) const`: if non-empty, write the top element to `out` and return `true`; otherwise return `false`.
- `int size() const`: return the number of elements.
- `static constexpr int capacity()`: return the template parameter `Capacity`.

The template is instantiated as `BoundedStack<int, 3>`, `BoundedStack<std::string, 2>`, `BoundedStack<double, 1>`, and others; the element type must be default-constructible, copy-constructible, and assignable.
