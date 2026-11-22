This exercise reads no stdin. Implement read-only statistics and trimming over a contiguous range.

```cpp
#include <cstddef>
#include <span>
#include <vector>

struct Stats {
    std::size_t count;
    long long sum;
    int min;
    int max;
};

Stats summarize(std::span<const int> values);
std::span<const int> trim_zeros(std::span<const int> values);
```

Rules:

- `summarize` reports the element count, the sum (accumulate in `long long` to avoid overflow), the minimum, and the maximum.
- Empty input returns `{0, 0, 0, 0}`.
- `trim_zeros` removes leading and trailing runs of zeros and returns a view over the original memory; an all-zero or empty input yields an empty view.
- The returned view must not own data and must not point into a newly created `std::vector`.
- Do not write `main`.

### Hint

`std::span::subspan(offset, count)` still points into the original buffer. The harness checks aliasing by comparing `data()` positions.
