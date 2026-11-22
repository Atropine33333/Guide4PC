This exercise reads no stdin and writes no stdout. A hidden harness includes your function template directly, so write it as a header-style implementation.

Implement:

```cpp
template <typename T>
T clamp_value(const T& value, const T& low, const T& high);
```

Return:

- `low` if `value < low`;
- `high` if `high < value`;
- otherwise `value`.

Callers guarantee `low <= high`. `T` is instantiated at least with `int`, `double`, and `char`. Do not write `main`.

Requirements:

- Use only `<` comparisons; do not require `T` to support arithmetic;
- When `low == high`, only one value is in range and the result must be it;
- The return type is `T` (returned by value).
