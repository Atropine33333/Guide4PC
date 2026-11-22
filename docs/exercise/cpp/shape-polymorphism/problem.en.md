Implement a family of shape classes.

Define an abstract base class `Shape`, then implement `Rectangle` and `Circle`:

- `Shape` declares pure virtual `double area() const` and `const char* name() const`, plus a virtual destructor.
- `Rectangle(double width, double height)`: area is `width * height`; `name()` returns `"rectangle"`.
- `Circle(double radius)`: area is `π * radius * radius`; `name()` returns `"circle"`. Use `3.14159265358979323846` for `π`.
- Both derived classes inherit `Shape` publicly and override those two virtual functions.

The functions must work when called through `Shape*`. Areas are compared with relative tolerance `1e-9`; names must match exactly, character by character.
