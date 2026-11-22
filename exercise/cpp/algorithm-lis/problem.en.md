Find the length of the longest strictly increasing subsequence of an integer sequence.

```cpp
#include <vector>

int lis_length(const std::vector<int>& values);
```

Rules:

- The subsequence need not be contiguous but must keep the original order.
- Strictly increasing: each element must be strictly greater than the previous one; equal values do not count.
- An empty sequence returns 0, and a single element returns 1.
- The sequence may hold up to `2·10^5` elements; an `O(n^2)` solution is too slow, so use `O(n log n)`.
- Do not write `main`.

### Example

| Call | Result |
| --- | --- |
| `lis_length({10, 9, 2, 5, 3, 7, 101, 18})` | `4` |
| `lis_length({1, 2, 3})` | `3` |
| `lis_length({2, 2, 2})` | `1` |

### Hint

Maintain an array `tails` where `tails[k]` is the smallest possible tail of an increasing subsequence of length `k + 1`. `tails` is itself strictly increasing; for each element use `std::lower_bound` to find the first position not less than it and replace that slot. The answer is the size of `tails`.
