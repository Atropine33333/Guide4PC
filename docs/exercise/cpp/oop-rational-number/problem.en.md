> The square field has its art of parts,
> the son and mother weighed and pressed.
> Subtract the less, and less again,
> divide by that which stands abreast.

The *Nine Chapters*, in the chapter on the square field, gives the art of reducing fractions: take the less from the greater, again and again, until the two are equal, then divide both by that equal number.

Now there are fractions: the top is called the son, the bottom the mother, and the mother is never zero. It is asked: reduced to its simplest, what form should it take? And what comes of adding, subtracting, multiplying and dividing two such numbers? And which of the two is greater, or are they equal?

The rule says: let the mother always be positive and let son and mother be coprime; then the form is simplest. If the mother be negative, change the signs of both; if the son be zero, let one stand as the mother. To add or subtract, first bring both to a common mother and then join their sons; to multiply, son by son and mother by mother; to divide, turn the second over and multiply. When done, reduce again by the equal number. To compare, cross-multiply and look upon the products; use no decimals.

**In plain words**: Implement the rational class `Rational`: a value is reduced as soon as it is constructed — the denominator is always positive and the two parts are coprime, `0` is written `0/1`, and `2/-4` becomes `-1/2`; arithmetic results are reduced as well; `==` and `<` compare values, and `<` cross-multiplies without floating point. Reduce with `std::gcd`.

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

### Requirements

- Every rational is stored in lowest terms: the denominator is always positive and the numerator and denominator are coprime;
- `0` is represented as `0/1`; input denominators may be negative, so `2/-4` becomes `-1/2`;
- Results of arithmetic must also be in lowest terms;
- `==` compares values, so `2/4` equals `1/2`;
- `<` compares values without floating point;
- Do not write `main`.

Hint: reduce with `std::gcd`; after ensuring positive denominators, compare `a/b < c/d` by cross-multiplying to `a*d < c*b`.
