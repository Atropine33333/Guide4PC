This exercise reads no stdin. Implement functions that only observe characters; every non-empty result must alias the original buffer.

```cpp
#include <string>
#include <string_view>

std::string_view trim(std::string_view text);
std::string_view first_word(std::string_view text);
std::string_view between(std::string_view text, char open, char close);
bool starts_with(std::string_view text, std::string_view prefix);
```

Rules:

- `trim` removes leading and trailing ASCII spaces `' '` and returns the rest; an all-space or empty input yields an empty view.
- `first_word` returns the span from the first non-space character to the next space or the end; it yields an empty view when there is no non-space character.
- `between` returns the span after the first `open` and before the next `close` after it; it yields an empty view when `open` is missing or has no following `close`.
- `starts_with` reports whether `prefix` is a prefix of `text`; an empty prefix counts as true.
- Non-empty results must point directly into `text`, never into a local object or a temporary `std::string`.
- Do not write `main`.

### Hint

A view returned by `std::string_view::substr` still points into the original buffer; watch for `std::string_view::npos` from `find`.
