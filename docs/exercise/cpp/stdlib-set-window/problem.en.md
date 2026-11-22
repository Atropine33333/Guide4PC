This task reads no stdin and writes no stdout. Implement two range-query functions over a `std::set<int>`.

```cpp
#include <cstddef>
#include <set>
#include <vector>

std::vector<int> window_values(const std::set<int>& values, int low, int high);

std::size_t window_count(const std::set<int>& values, int low, int high);
```

Rules:

- `window_values` returns every element `x` in the set satisfying `low <= x <= high`, in ascending order.
- `window_count` returns the number of such elements without building the result vector.
- The range is inclusive: both `low` and `high` are included.
- If `low > high`, or no element lies in the range, return an empty `vector` or `0` respectively.
- Prefer `lower_bound` and `upper_bound` so a query does not scan the entire set.
- Do not write `main`, and do not change the signatures.
