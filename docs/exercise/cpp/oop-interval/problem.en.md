Implement a closed interval class.

```cpp
class Interval {
public:
    Interval(int low, int high);
    int low() const;
    int high() const;
    bool contains(int value) const;
    bool overlaps(const Interval& other) const;
    Interval united(const Interval& other) const;
    bool operator==(const Interval& other) const;
};
```

Rules:

- If `low > high` at construction, swap the endpoints so that `low() <= high()`.
- Intervals are closed: `contains` returns `true` at the endpoints too.
- `overlaps` reports whether two closed intervals share a point; touching endpoints (`a.high() == b.low()`) count as overlapping.
- `united` returns the smallest closed interval containing both, even when they are disjoint (the gap is included).
- `operator==` returns `true` exactly when both endpoints are equal.
- Do not write `main`.

### Example

| Call | Result |
| --- | --- |
| `Interval(5, 2).low()` / `.high()` | `2` / `5` |
| `Interval(1, 4).contains(4)` | `true` |
| `Interval(1, 2).overlaps(Interval(2, 5))` | `true` |
| `Interval(1, 3).united(Interval(5, 7))` | `[1, 7]` |

### Hint

Use a member initializer list with `std::min`/`std::max` or a conditional swap in the constructor. `overlaps` can be written `!(high_ < other.low_ || other.high_ < low_)`. `united` returns `Interval(std::min(low_, other.low_), std::max(high_, other.high_))`.
