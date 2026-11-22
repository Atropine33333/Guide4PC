The Code of Integer Parsing

Article 1 (Scope) This Code governs the parsing of a piece of text into an `int`. A failed parse shall not throw; it shall return `std::nullopt`.

Article 2 (Valid Spelling) Valid text consists of one optional `-` sign followed by at least one decimal digit, and nothing else.

Article 3 (Prohibitions) A `+` sign, spaces, a decimal point, and letters are all invalid; the empty string is invalid; a lone `-` is also invalid.

Article 4 (Leading Zeros) Leading zeros are permitted, and `"000123"` parses as 123; `"-0"` is valid and equals 0.

Article 5 (Range) The parsed result must lie between `-2147483648` and `2147483647`. On overflow, return `std::nullopt`; wrap-around shall not be relied upon.

Article 6 (`parse_or`) `parse_or` returns the parsed value on success and the caller's `fallback` on failure.

Article 7 (Supplementary) Implement the two functions below and do not write `main`. Matters not covered by this Code follow the semantics of `std::optional<int>`.

```cpp
#include <optional>
#include <string>

std::optional<int> parse_int(const std::string& text);
int parse_or(const std::string& text, int fallback);
```

**In plain words**: Implement `parse_int` and `parse_or`. `parse_int` returns an integer only when the text is exactly an optional minus sign followed by at least one digit and the value fits in `int`; otherwise it returns `std::nullopt`, never throwing. `parse_or` returns the parsed value on success and `fallback` on failure. Character-by-character checking suits this better than `std::stoi`, which throws; accumulate digits in a `long long` and check the range at the end.

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
