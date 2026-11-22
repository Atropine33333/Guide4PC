This exercise reads no stdin and writes no stdout. Implement integer parsing that never throws, tested by a hidden harness.

```cpp
#include <optional>
#include <string>

std::optional<int> parse_int(const std::string& text);
int parse_or(const std::string& text, int fallback);
```

Rules:

- A valid spelling is one optional `-` followed by at least one digit and nothing else.
- A `+` sign, spaces, a dot, or letters are not allowed; the empty string and a lone `-` are invalid.
- Leading zeros are allowed (`"000123"`), and `"-0"` is valid and equals 0.
- The result must fit in `int` (`-2147483648` to `2147483647`); return `std::nullopt` on overflow instead of relying on wrap-around.
- `parse_or` returns the parsed value on success and `fallback` otherwise.
- Do not write `main`.

### Example

| Call | Result |
| --- | --- |
| `parse_int("42")` | `42` |
| `parse_int("-0")` | `0` |
| `parse_int("4x")` | `std::nullopt` |
| `parse_int("2147483648")` | `std::nullopt` |
| `parse_or("abc", -1)` | `-1` |

### Hint

Checking character by character is a better fit than `std::stoi`: `std::stoi` throws, and exceptions are not allowed here. Accumulate digits in a `long long` and check the range at the end.
