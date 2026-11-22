Implement two functions that place text into a fixed-width field.

```cpp
#include <cstddef>
#include <string>

std::string truncate_field(const std::string& text, std::size_t width);

std::string center_field(const std::string& text, std::size_t width);
```

Rules:

- `truncate_field`:
  - if `text.size() <= width`, return the original string without padding;
  - if `text.size() > width`, keep only the first `width` characters;
  - if `width == 0`, return an empty string.
- `center_field`:
  - if `text.size() >= width`, return the same result as `truncate_field(text, width)`;
  - otherwise pad with spaces on both sides so the result is exactly `width` characters long;
  - the two pad counts differ by at most 1; if the total padding is odd, the right side gets the extra space (for a total padding of 3, use 1 space on the left and 2 on the right).
- The input text contains only printable ASCII characters; UTF-8 and display width are out of scope.
- Do not write `main`, and do not change the signatures.
