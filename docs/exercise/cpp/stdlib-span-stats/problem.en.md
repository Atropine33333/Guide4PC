Memory Resource Management Office

Document No. 8 (2026)

Notice on Read-Only Statistics and Trimming of Contiguous Memory Segments

To all spans and all views:

To further standardize read-only inspection of contiguous memory segments and to eliminate out-of-bounds access, confused ownership, and dangling views, this Office has decided to conduct the present special review. The relevant matters are set out below.

1. Each unit shall implement the functions declared below, and shall not add to or alter the signatures.

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

2. `summarize` shall report the element count, the sum, the minimum, and the maximum; the sum must accumulate in `long long` to prevent overflow. Empty input returns `{0, 0, 0, 0}`.

3. `trim_zeros` shall remove the leading and trailing runs of zeros and return a subview over the original memory; when every element is zero, or the input is empty, it returns an empty view.

4. A returned view shall not own its data and shall not point into a newly created `std::vector`.

5. Do not write `main`.

This Notice takes effect on the date of issue.

Memory Resource Management Office
22 November 2026

Cc: Read-Only View Supervision Committee, Pointer Aliasing Inspection Group.

**In plain words**: Implement `summarize` and `trim_zeros`. `summarize` reports the element count, sum, minimum, and maximum, returning `{0, 0, 0, 0}` for empty input and accumulating the sum in `long long`. `trim_zeros` removes leading and trailing zeros and returns a `std::span` subview over the original buffer; an all-zero or empty input yields an empty view. The returned view must not own data or point into a temporary container. `subspan(offset, count)` still points into the original buffer and can be used for trimming.
