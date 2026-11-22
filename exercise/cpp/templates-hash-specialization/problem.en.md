This exercise reads no stdin. Give a custom 2-D point type equality and a `std::hash` specialisation so it can be used in `std::unordered_set`.

```cpp
#include <cstddef>
#include <functional>

struct Point {
    int x;
    int y;
    bool operator==(const Point& other) const;
};

namespace std {
template <>
struct hash<Point> {
    std::size_t operator()(const Point& point) const;
};
}
```

Rules:

- Points with both coordinates equal are equal; swapping the coordinates makes them unequal.
- Equal points must produce equal hash values.
- The hash must depend on both `x` and `y`: swapping the coordinates, or changing only one coordinate, must produce a different hash for the tested points (rare collisions are allowed in general, but the test points must be distinguished).
- Negative coordinates are supported.
- Do not write `main`.

### Hint

One simple combination is `std::hash<int>{}(x) * 1000003u ^ std::hash<int>{}(y)`; shifts and xor mixing also work. `std::hash<Point>` is a specialisation of a standard template and must be written inside `namespace std`.
