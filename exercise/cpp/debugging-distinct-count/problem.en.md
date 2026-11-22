The function below is supposed to count distinct values in a sorted sequence, but its result is always off by one. Fix it so that every input returns the correct count.

```cpp
#include <vector>

int count_distinct(const std::vector<int>& sorted);
```

Rules and constraints:

- `sorted` is a non-decreasing sequence, possibly with duplicates and possibly empty.
- Return the number of distinct values.
- Do not change the signature and do not write `main`.
- Only modify the implementation in `starter.cpp`.
