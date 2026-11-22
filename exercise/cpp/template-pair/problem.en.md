This exercise reads no stdin and writes no stdout. Implement a generic pair class template; a hidden harness runs the tests.

Implement the class template `Pair<T>`:

- `Pair()`: value-initializes both elements.
- `Pair(const T& first, const T& second)`: constructs from the given values.
- `T& first()` / `const T& first() const`: access the first element; both overloads are required.
- `T& second()` / `const T& second() const`: access the second element; both overloads are required.
- `Pair<T> swapped() const`: return a new pair with the elements exchanged, leaving the original unchanged.
- `bool operator==(const Pair<T>& other) const`: return `true` when both elements are equal.

The harness instantiates the template with `int`, `std::string`, and `Pair<int>`; a `const` object must also allow reading its elements.
