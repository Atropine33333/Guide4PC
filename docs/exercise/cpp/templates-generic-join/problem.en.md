This exercise reads no stdin and writes no stdout. Write a join function that works with any iterable container, tested by a hidden harness.

```cpp
#include <string>

template <typename Container>
std::string join(const Container& items, const std::string& separator);
```

Rules:

- Write every element into the result with `operator<<`, inserting `separator` between elements.
- No separator before the first element or after the last one.
- An empty container returns an empty string.
- The container may be `std::vector<int>`, `std::vector<std::string>`, `std::deque<int>`, or anything else that supports a range `for`.
- Do not write `main`.

### Example

| Call | Result |
| --- | --- |
| `join(std::vector<int>{1, 2, 3}, ", ")` | `1, 2, 3` |
| `join(std::vector<std::string>{"a", "b"}, "-")` | `a-b` |
| `join(std::vector<int>{}, ",")` | an empty string |
| `join(std::vector<int>{4, 5}, "")` | `45` |

### Hint

Collect into a `std::ostringstream` and iterate with `for (const auto& item : items)`. Track a `bool first` so no trailing separator is emitted. Templates may live in a header; here everything is in one translation unit anyway.
