Implement three lookup and comparison functions.

```cpp
#include <cstddef>
#include <string>
#include <vector>

std::size_t common_prefix(const std::string& a, const std::string& b);
long long last_index_of(const std::vector<int>& values, int target);
bool is_rotation(const std::string& a, const std::string& b);
```

Rules:

- `common_prefix` returns the length of the common prefix of the two strings; it is `0` when either is empty or the first characters differ.
- `last_index_of` returns the index of the last occurrence of `target` in `values`; it returns `-1` when absent, including for an empty vector.
- `is_rotation` reports whether `b` is a cyclic shift of `a`; different lengths yield `false`, and two empty strings count as rotations of each other.
- Do not write `main`.

### Hint

`std::mismatch` walks two ranges and stops at the first difference. For `is_rotation`, compare lengths first and then use `(a + a).find(b)`; `find("")` always returns `0`, so the length check is essential.
