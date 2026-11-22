*Night-Shift Rules for the Shape Warehouse*

You are hired to watch a warehouse of shapes. Every rule below is true; common sense, however, does not always apply here. Follow them.

1. Shapes enter and leave under the name `Shape*`; deletion also goes through `Shape*`, never through a downcast.

2. The destructor of `Shape` must be virtual. If it is not, deleting a derived object through `Shape*` leaves the derived destructor uncalled — the standard calls this undefined behaviour; this warehouse calls it "the thing goes missing".

3. If you leave the base destructor non-virtual, every rule from the fourth onward is void, for the program may never reach them.

4. From here on the sheet holds, each line true: `Rect::label()` reports `"rect"` and its area is `width * height`; `Circle::label()` reports `"circle"` and its area is `π * radius * radius`, with `π = 3.14159265358979323846`; dimensions may be `0`, and then the area is `0`.

5. Deleting a derived object through a `Shape*` must run the derived destructor.

6. No `main` shall be written.

**In plain words**: Implement the shape hierarchy: `Shape` declares the pure virtual `std::string label() const` and `double area() const`, and its destructor must be virtual; `Rect(width, height)` reports `"rect"` with area `width * height`; `Circle(radius)` reports `"circle"` with area `π * radius * radius`.

```cpp
#include <string>

class Shape {
public:
    virtual ~Shape() = default;  // must stay virtual
    virtual std::string label() const = 0;
    virtual double area() const = 0;
};

class Rect : public Shape {
public:
    Rect(double width, double height);
    std::string label() const override;
    double area() const override;
};

class Circle : public Shape {
public:
    explicit Circle(double radius);
    std::string label() const override;
    double area() const override;
};
```

### Rules

- `Rect::label()` returns `"rect"` and its area is `width * height`.
- `Circle::label()` returns `"circle"` and its area is `pi * radius * radius`, with `pi = 3.14159265358979323846`.
- Dimensions may be `0`, in which case the area is `0`.
- `Shape`'s destructor must be `virtual` (or a virtual `= default` function); a derived object deleted through a `Shape*` must run its own destructor.
- Do not write `main`.

### Hint

The starter's `~Shape()` is not declared virtual yet; change it to `virtual ~Shape() = default;`. A derived destructor cannot be marked `override` until the base destructor is virtual.
