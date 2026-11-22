Implement:

```cpp
#include <cstddef>
#include <vector>

std::vector<double> window_average(const std::vector<int>& values,
                                   std::size_t window);
```

Return the averages of all consecutive windows of length `window`, ordered by their starting position. Count only complete windows: if `window == 0` or `window > values.size()`, return an empty `vector`.

Element `i` of the result is `values[i] + ... + values[i + window - 1]` divided by `window`, computed as `double`.

The function in `starter.cpp` already exists, but its running sum never subtracts the element leaving the window. Fix that bug without changing the interface. Do not write `main`.
