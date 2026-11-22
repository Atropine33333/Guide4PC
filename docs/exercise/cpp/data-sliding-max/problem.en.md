Implement a function that returns the maximum of every contiguous window of length `window`.

```cpp
#include <cstddef>
#include <vector>

std::vector<long long> sliding_max(const std::vector<long long>& values,
                                   std::size_t window);
```

Rules:

- The result length must be `values.size() - window + 1` when `1 <= window <= values.size()`.
- Return an empty `vector` when `window == 0` or `window > values.size()`.
- Windows are ordered from left to right; duplicates and negative values are allowed.
- Aim for `O(n)` time; use a monotonic deque instead of rescanning each window.
- Do not write `main`, and do not change the signature.
