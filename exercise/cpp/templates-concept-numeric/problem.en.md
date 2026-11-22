This exercise reads no stdin and writes no stdout. Constrain a template parameter with a C++20 concept and implement an average function, tested by a hidden harness.

```cpp
#include <concepts>
#include <type_traits>
#include <vector>

template <typename T>
concept Numeric = std::is_arithmetic_v<T> && !std::is_same_v<T, bool>;

template <Numeric T>
T average_of(const std::vector<T>& values, T fallback);
```

Rules:

- `Numeric` accepts every arithmetic type except `bool`; `Numeric<int>`, `Numeric<double>`, and `Numeric<unsigned>` are true, while `Numeric<bool>` and `Numeric<std::string>` are false.
- `average_of` returns `fallback` for an empty sequence.
- Otherwise it returns the average of all elements, computing everything in `T`: integer division truncates toward zero, so `{1, 2}` averages to `1` and `{-1, -2}` to `-1`.
- Do not write `main`.

### Example

| Call | Result |
| --- | --- |
| `average_of<int>({1, 2}, 0)` | `1` |
| `average_of<int>({}, 42)` | `42` |
| `average_of<double>({1.5, 2.5}, 0.0)` | `2.0` |
| `average_of<unsigned>({1u, 2u}, 0u)` | `1u` |

### Hint

Use `std::is_arithmetic_v` for arithmetic types and `std::is_same_v<T, bool>` to exclude `bool`. Keep the sum and the count in `T`; convert the length with `static_cast<T>(values.size())`. A concept can go straight into the template parameter list: `template <Numeric T>`.
