CODE OF NUMERIC TYPES

Article 1. "Numeric type" in this Code means an arithmetic type other than `bool`. `int`, `double` and `unsigned` fall within it; `bool` and `std::string` fall outside.

Article 2. Every template that accepts a numeric type shall constrain its parameter with `Numeric`, and shall not substitute a run-time check.

Article 3. An average shall be computed in its own type `T` from first to last. The sum of the elements is kept in `T`, and the count is converted to `T`.

Article 4. For an empty sequence, return the `fallback` supplied by the caller.

Article 5. Otherwise return the average of all elements. Integer division truncates toward zero: the average of `{1, 2}` is `1`, and of `{-1, -2}` is `-1`.

Article 6. This Code takes effect when the template is instantiated.

**In plain words**: define the concept `Numeric` (arithmetic types, excluding `bool`), then implement `average_of`: return `fallback` for an empty sequence, and compute the whole average in `T` otherwise, with integer division truncating toward zero.

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
