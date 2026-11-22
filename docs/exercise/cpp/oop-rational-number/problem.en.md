This exercise reads no stdin and writes no stdout. Implement a rational number class; a hidden harness runs the tests.

Implement:

```cpp
class Rational {
public:
    Rational(int numerator, int denominator);  // denominator != 0

    int numerator() const;
    int denominator() const;

    Rational operator+(const Rational& other) const;
    Rational operator-(const Rational& other) const;
    Rational operator*(const Rational& other) const;
    Rational operator/(const Rational& other) const;  // other is nonzero

    bool operator==(const Rational& other) const;
    bool operator<(const Rational& other) const;
};
```

Requirements:

- Every rational is stored in lowest terms: the denominator is always positive and the numerator and denominator are coprime;
- `0` is represented as `0/1`; input denominators may be negative, so `2/-4` becomes `-1/2`;
- Results of arithmetic must also be in lowest terms;
- `==` compares values, so `2/4` equals `1/2`;
- `<` compares values without floating point;
- Do not write `main`.

Hint: reduce with `std::gcd`; after ensuring positive denominators, compare `a/b < c/d` by cross-multiplying to `a*d < c*b`.
