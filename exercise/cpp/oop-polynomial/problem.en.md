This task reads no stdin and writes no stdout. Implement a dense polynomial class with 64-bit integer coefficients.

```cpp
#include <vector>

class Polynomial {
public:
    Polynomial();
    explicit Polynomial(std::vector<long long> coefficients);

    int degree() const;
    long long coefficient(int power) const;
    long long evaluate(long long x) const;

    Polynomial operator+(const Polynomial& other) const;
    Polynomial operator*(const Polynomial& other) const;
    bool operator==(const Polynomial& other) const;
};
```

Rules:

- The `coefficients` constructor argument is ordered from lowest degree to highest: index `i` is the coefficient of `x^i`.
- The zero polynomial has degree `-1`; a nonzero polynomial's degree is the largest index with a nonzero coefficient.
- Construction and arithmetic results must trim high zero coefficients, so `{1, 2, 0}` and `{1, 2}` represent the same polynomial.
- `coefficient(power)` returns 0 for negative or out-of-range powers.
- `evaluate(x)` may use Horner's method or an equivalent computation.
- `operator+` and `operator*` return normalized new objects; the zero polynomial must work correctly in both operations.
- `operator==` compares values and ignores high zero coefficients.
- Do not write `main`, and do not change the public interface.
