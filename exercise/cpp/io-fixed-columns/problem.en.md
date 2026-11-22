Implement fixed-column slicing and parsing.

```cpp
#include <string>

struct Person {
    std::string name;
    int id;
    int score;
};

std::string slice(const std::string& line, int start, int width);
Person parse_person(const std::string& line);
```

Every line uses the same fixed column layout:

| Field | Start column | Width |
| --- | --- | --- |
| `id` | 0 | 4 |
| `name` | 4 | 10 |
| `score` | 14 | 6 |

Rules:

- `slice` returns at most `width` characters starting at column `start`, with leading and trailing ASCII spaces removed.
- If `start < 0`, `width <= 0`, or `start` is already past the end of the line, `slice` returns an empty string.
- A line may be shorter than 20 characters: missing columns are treated as blanks.
- `parse_person` extracts the three fields with `slice`; a blank `id` or `score` becomes `0`; `name` keeps interior spaces but loses leading and trailing ones.
- Do not write `main`.

### Hint

Use `substr` and then skip spaces at both ends by hand. Numeric fields may have leading spaces, but their content is guaranteed to be a valid decimal integer (or empty).
