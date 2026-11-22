The disciples said to Jesus: "This is a lonely place, and the crowd is seated; the loaves and fishes are all given out, but fragments remain on the ground."

Jesus said: "Gather up the fragments that remain, that nothing be lost."

So they took baskets and gathered them, piece by piece. Between two pieces they laid a `separator`; before the first they laid none, and after the last they laid none.

One asked: "Do the baskets hold only bread?"

Jesus said: "Not only. Whole numbers are gathered, and words are gathered; whatever can be taken out piece by piece is gathered."

Another asked: "And an empty basket?"

Jesus said: "An empty basket comes in, and an empty basket goes out."

And when they had gathered, they counted, and nothing was lost.

**In plain words**: write a `join` function that works with any iterable container: write every element with `operator<<`, put `separator` between elements and nowhere at the ends, and return an empty string for an empty container.

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
