This exercise reads no stdin and writes no stdout. Implement two functions that convert integers between arbitrary bases (2 to 36); a hidden harness runs the tests.

```cpp
#include <string>

std::string to_base(long long value, int base);
bool parse_base(const std::string& text, int base, long long& out);
```

Rules:

- The base must satisfy `2 ≤ base ≤ 36`; when it does not, `to_base` returns an empty string and `parse_base` returns `false`.
- `to_base` writes `value` in that base: digits above 9 use uppercase `A`–`Z`, `0` becomes `"0"`, and a negative value gets a leading `-`.
- `to_base` must handle `LLONG_MIN` (`-9223372036854775808`) without negating it directly.
- `parse_base` accepts an optional `+` or `-` sign followed by at least one digit; letters may be upper or lower case.
- If `text` contains an illegal character, has no digits, or does not fit in `long long`, return `false` and leave `out` unchanged.
- On success `parse_base` writes the result to `out` and returns `true`.
- Do not write `main`.

### Example

| Call | Result |
| --- | --- |
| `to_base(255, 16)` | `"FF"` |
| `to_base(-5, 2)` | `"-101"` |
| `to_base(0, 36)` | `"0"` |
| `parse_base("101", 2, out)` | `true`, `out == 5` |
| `parse_base("-ff", 16, out)` | `true`, `out == -255` |
| `parse_base("12", 2, out)` | `false` |

### Hint

Convert a negative value to its magnitude as an `unsigned long long` before taking remainders, and add the sign at the end. While parsing, accumulate in `unsigned long long` and check for overflow before multiplying; the limit is `LLONG_MAX`, or `1ULL << 63` for negative values.
