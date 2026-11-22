This exercise reads no stdin. Implement a shape hierarchy and destroy derived objects correctly through a base pointer.

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

Rules:

- `Rect::label()` returns `"rect"` and its area is `width * height`.
- `Circle::label()` returns `"circle"` and its area is `pi * radius * radius`, with `pi = 3.14159265358979323846`.
- Dimensions may be `0`, in which case the area is `0`.
- `Shape`'s destructor must be `virtual` (or a virtual `= default` function); the harness defines its own derived class and deletes it through a `Shape*` to check that the derived destructor runs.
- Do not write `main`.

### Hint

The starter's `~Shape()` is not virtual yet; change it to `virtual ~Shape() = default;`. A derived destructor cannot be marked `override` until the base destructor is virtual.
