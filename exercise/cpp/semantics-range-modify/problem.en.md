This exercise reads no stdin and writes no stdout. Implement three functions over integer sequences, tested by a hidden harness.

```cpp
#include <vector>

void scale_in_place(std::vector<int>& values, int factor);
long long sum_positive(const std::vector<int>& values);
int count_multiples(const std::vector<int>& values, int divisor);
```

Rules:

- `scale_in_place` multiplies every element of `values` by `factor` in place; `factor` may be zero or negative. The test data keeps every result inside the `int` range.
- `sum_positive` returns the sum of all elements strictly greater than 0 as a `long long`; an empty sequence yields 0. It takes a `const` reference and must not modify the sequence.
- `count_multiples` returns how many elements are divisible by `divisor` (`value % divisor == 0`); `divisor` is never 0; an empty sequence yields 0.
- Do not write `main`.

### Hint

Use `for (int& value : values)` rather than `for (int value : values)`; the latter changes a copy. Accumulate `sum_positive` in a `long long` so that several `int` values cannot overflow the sum.
