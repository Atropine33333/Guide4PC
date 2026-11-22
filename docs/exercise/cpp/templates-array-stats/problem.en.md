This exercise reads no stdin and writes no stdout. Implement two function templates that take array references; a hidden harness runs the tests.

```cpp
template <typename T, int N>
double average(const T (&values)[N]);

template <typename T, int N>
T max_value(const T (&values)[N]);
```

Rules:

- Both templates deduce `T` and `N` from an array reference, with `N >= 1`.
- `average` returns the arithmetic mean as a `double`; even when `T` is integral, the result must not be truncated by integer division.
- `max_value` returns the largest element with type `T`, comparing with `<`; when values are equal either one may be returned.
- `int`, `double`, `long long`, and `char` arrays must support both `average` and `max_value`; `std::string` arrays only need `max_value`.
- Do not write `main`.

### Hint

Accumulate elements into a `double` while iterating, then divide by `N`. Never store a sum of `char` back into a `char`, or it overflows. A `std::string` array is only tested with `max_value`; `average` is never instantiated for it.
