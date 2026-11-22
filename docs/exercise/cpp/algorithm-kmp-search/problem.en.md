This task reads no stdin and writes no stdout. Implement a function that returns the starting indices of every occurrence of a pattern in a text.

```cpp
#include <cstddef>
#include <string>
#include <vector>

std::vector<std::size_t> find_all(const std::string& text,
                                  const std::string& pattern);
```

Rules:

- Return every starting index `pos` such that `text[pos + i] == pattern[i]` for all valid `i`, in ascending order.
- Overlaps are allowed. For example, searching for `"aaa"` in `"aaaaa"` returns `{0, 1, 2}`.
- Return an empty `vector` for an empty pattern.
- Return an empty `vector` when the pattern is longer than the text.
- The strings contain ASCII characters only and comparisons are case-sensitive.
- Use KMP (the prefix function); do not call `std::string::find` repeatedly.
- Do not write `main`, and do not change the signature.
