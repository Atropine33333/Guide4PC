This exercise reads no stdin and writes no stdout. Implement an RGB pixel class; a hidden harness runs the tests.

Implement:

```cpp
class Pixel {
public:
    Pixel(int red, int green, int blue);
    int red() const;
    int green() const;
    int blue() const;
    void set_red(int red);
    void set_green(int green);
    void set_blue(int blue);
    int brightness() const;  // (red + green + blue) / 3, integer division
    bool is_black() const;   // whether all three components are 0
};
```

Requirements:

- Every component is clamped into the closed range `0..255` during construction and assignment; inputs may lie outside it;
- `brightness()` uses integer division and stays within `0..255`;
- `is_black()` returns `true` only when all three components are 0.

Do not write `main`.
