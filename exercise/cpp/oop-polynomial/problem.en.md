> The heavenly element is set as the form,
> its rods laid out from low to high.
> Trim the empty zeros above,
> and sums and products follow thereby.

The old art of the heavenly element sets one as the element and lays out counting rods by degree: lower degrees first, higher degrees after, blank places filled with zero.

Now there are forms whose coefficients are 64-bit numbers, laid out in a row from the lowest degree to the highest. It is asked: what is the degree of the form? What is the coefficient of a given degree? What value does the form take at a given number? And when two forms are added or multiplied, what form comes forth?

The rule says: the degree is the place of the highest nonzero rod; a form all of zeros is recorded with degree `-1`. A coefficient asked beyond the row is reported as zero. To evaluate at a number, walk from the highest degree downward, at each rod multiply by the element and add the rod — this is called Horner. To add, add rods of the same place; to multiply, climb each place against every other and gather the products. Whatever comes forth is trimmed of its high zeros before it may be called a form.

**In plain words**: Implement the dense polynomial class `Polynomial` with `long long` coefficients: the constructor argument is ordered from the lowest degree to the highest, and index `i` is the coefficient of `x^i`; the zero polynomial has degree `-1`, and a nonzero form takes the index of its highest nonzero coefficient; construction and arithmetic results trim high zero coefficients, so `{1, 2, 0}` and `{1, 2}` are the same form; `coefficient` returns 0 for out-of-range powers; `evaluate` uses Horner's method; `+` and `*` return normalized new objects; `==` compares values.

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

### Rules

- The `coefficients` constructor argument is ordered from lowest degree to highest: index `i` is the coefficient of `x^i`.
- The zero polynomial has degree `-1`; a nonzero polynomial's degree is the largest index with a nonzero coefficient.
- Construction and arithmetic results must trim high zero coefficients, so `{1, 2, 0}` and `{1, 2}` represent the same polynomial.
- `coefficient(power)` returns 0 for negative or out-of-range powers.
- `evaluate(x)` may use Horner's method or an equivalent computation.
- `operator+` and `operator*` return normalized new objects; the zero polynomial must work correctly in both operations.
- `operator==` compares values and ignores high zero coefficients.
- Do not write `main`, and do not change the public interface.
