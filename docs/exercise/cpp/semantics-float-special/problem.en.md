ZHUANGZI, BOOK XVII, CONTINUED

Huizi said to Zhuangzi: "A rod one foot long, halved each day, will never be exhausted in ten thousand ages. Can a finite vessel then measure an infinite number?"

Zhuangzi said: "It can. The vessel has fixed names: the infinite, the not-a-number, and the negative zero. The infinite exceeds every finite thing, and adding one leaves it infinite. The not-a-number equals nothing; it does not even equal itself. The negative zero has value zero and a negative sign: it equals positive zero, yet its sign can be told."

Huizi said: "If it does not equal itself, how is one to treat it?"

Zhuangzi said: "First ask whether it is ordered. What is ordered is compared by magnitude; what is unordered falls into one class, and precedence is no longer discussed. If you would know numbers, know their vessels first."

**In plain words**: Implement six functions over IEEE-754 special values: detect NaN, detect finite values, detect negative zero, compare with `==`, give a three-way order (returning 2 when unordered), and divide safely with a fallback when the divisor is zero.

```cpp
#include <cmath>

bool is_nan_value(double x);
bool is_finite_value(double x);
bool is_negative_zero(double x);
bool same_value(double a, double b);
int total_order(double a, double b);
double safe_div(double a, double b, double fallback);
```

### Rules

- `is_nan_value`: true when `x` is NaN.
- `is_finite_value`: true when `x` is neither infinite nor NaN.
- `is_negative_zero`: true when `x` is zero with a negative sign bit (`-0.0` is true, `+0.0` is false).
- `same_value`: compare with `==`; thus `+0.0` equals `-0.0`, and no NaN equals anything, including itself.
- `total_order`: return `-1` when `a < b`, `1` when `a > b`, `0` when `a == b`, and `2` when either operand is NaN (unordered).
- `safe_div`: return `fallback` when `b` equals zero (including `-0.0`), otherwise return `a / b`. `a` and `b` may be ordinary finite numbers, infinities, or NaN.
- Do not write `main`.

### Hint

Use `std::isnan`, `std::isfinite`, and `std::signbit` from `<cmath>`. NaN compares false against everything, so the order of the four tests in `total_order` matters: compare less, compare greater, compare equal, then fall through to unordered.
