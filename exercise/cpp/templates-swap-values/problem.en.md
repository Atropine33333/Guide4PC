This exercise reads no stdin and writes no stdout. Implement two function templates; a hidden harness runs the tests.

```cpp
template <typename T>
void swap_values(T& first, T& second);

template <typename T>
T min_value(T first, T second);
```

Rules:

- `swap_values` exchanges the values of two objects of the same type; it must allow both references to bind to the same object, which changes nothing.
- `min_value` returns the smaller of two same-typed values; when they are equal either one may be returned.
- Both functions must work for `int`, `double`, `char`, `std::string`, and similar types, using only `<` for comparison.
- Do not use `std::swap`; implement the exchange yourself with a temporary variable.
- Do not write `main`.

### Hint

The template parameter `T` is deduced from the arguments; both arguments must have the same type. For strings, `<` is lexicographic comparison.
