(The diviner lays `N` cards face up in a single row.)

Querent: What does this row ask?

Diviner: Two things. First, the average of the row — gather every card's number into one heap, then divide it by the number of cards. Second, the heaviest card in the row.

Querent: Will the average lose its fraction because the cards are whole numbers?

Diviner: It will, so the reader must keep a `double` abacus. Add the integers first and divide afterwards, and the fraction survives.

Querent: By what measure is the heaviest card found?

Diviner: Only by `<`. Compare card with card, and the greatest rises to the top. Two of equal weight read either way; the faces show no difference.

Querent: And if the cards are words?

Diviner: Words compare too, in dictionary order. But words are only asked for the heaviest; averaging words is not a reading this spread performs.

**In plain words**: implement two function templates, `average` and `max_value`, that take array references and deduce `T` and `N`; `N >= 1`, `average` returns `double` without truncation by integer division, and `max_value` picks the largest by `<`.

```cpp
template <typename T, int N>
double average(const T (&values)[N]);

template <typename T, int N>
T max_value(const T (&values)[N]);
```

Rules:

- Both templates deduce `T` and `N` from an array reference, with `N >= 1`.
- `average` returns the arithmetic mean as a `double`; even when `T` is integral, the result must not be truncated by integer division.
- `max_value` returns the largest element with type `T`, comparing with `<`; when values are equal either one may be returned.
- `int`, `double`, `long long`, and `char` arrays must support both `average` and `max_value`; `std::string` arrays only need `max_value`.
- Do not write `main`.

### Hint

Accumulate elements into a `double` while iterating, then divide by `N`. Never store a sum of `char` back into a `char`, or it overflows. A `std::string` array is only tested with `max_value`; `average` is never instantiated for it.
