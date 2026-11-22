This exercise reads no stdin. Implement a reusable range reduction driven by policy classes.

```cpp
#include <vector>

struct SumPolicy {
    static int combine(int left, int right);
    static int identity();
};

struct MaxPolicy {
    static int combine(int left, int right);
    static int identity();
};

template <class Policy>
int reduce(const std::vector<int>& values);
```

Rules:

- `SumPolicy::identity()` returns `0`, and its `combine` returns the sum.
- `MaxPolicy::identity()` returns `INT_MIN`, and its `combine` returns the larger value.
- `reduce<Policy>` starts from `Policy::identity()` and performs `acc = Policy::combine(acc, value)` from left to right.
- An empty sequence returns `Policy::identity()`.
- Do not write `main`.

The harness also defines its own policy class (for example a product policy) and calls `reduce<that policy>`, so do not hard-code summation or maximisation.

### Hint

`reduce` lives in the header, and its template argument only needs the two static members `identity()` and `combine(int, int)`.
