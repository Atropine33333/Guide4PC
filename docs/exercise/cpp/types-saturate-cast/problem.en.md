Implement saturating conversions that never trigger undefined behaviour.

```cpp
#include <cmath>
#include <limits>

int saturate_to_int(long long x);
short saturate_to_short(long long x);
int saturate_double_to_int(double x);
```

Rules:

- `saturate_to_int`: clamp `x` to the `int` range `[INT_MIN, INT_MAX]`; values inside the range are returned unchanged.
- `saturate_to_short`: clamp `x` to the `short` range `[-32768, 32767]`; values inside the range are returned unchanged.
- `saturate_double_to_int`:
  - return `0` when `x` is NaN;
  - return `INT_MAX` for positive infinity and `INT_MIN` for negative infinity;
  - otherwise truncate toward zero, then clamp to the `int` range.

Requirements:

- Do not cast a `long long` or `double` straight to a narrower type and check afterwards; that is exactly where undefined behaviour happens.
- Use `std::numeric_limits` and `<cmath>` checks instead of hard-coded platform-dependent magic numbers (`short` is at least 16 bits; this exercise assumes the common 16-bit `short` and 32-bit `int`).
- Do not write `main`.

### Hint

Widen the boundary before comparing, or compare directly with `std::numeric_limits<T>::max()`. For `double`, `static_cast<double>(INT_MAX)` is an exactly representable safe upper bound.
