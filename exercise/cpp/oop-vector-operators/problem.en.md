Implement a 2D vector type with operators.

Implement:

```cpp
struct Vec2 {
    double x;
    double y;

    Vec2(double x = 0.0, double y = 0.0);

    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator*(double scalar) const;
    Vec2 operator-() const;                  // unary minus
    Vec2& operator+=(const Vec2& other);
    bool operator==(const Vec2& other) const;

    double dot(const Vec2& other) const;
    double length_squared() const;
};

Vec2 operator*(double scalar, const Vec2& value);  // supports 2.0 * v
```

Requirements:

- All operations work component by component;
- Unary minus negates both components;
- `+=` modifies the left operand and returns a reference to it;
- `==` compares components exactly (all test values are exactly representable as `double`);
- `length_squared()` returns `x*x + y*y` without taking a square root;
- Do not write `main`.
