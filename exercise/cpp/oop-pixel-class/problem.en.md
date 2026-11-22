Late-night television. The lights come up.

"Look here, friend! One pixel, three channels — red, green, blue, each with its own lane! Input hands you a number past `255`? No problem: we clamp it into `0..255` for you, not a bit over, not a bit under!

"Average brightness? Add all three, divide by three as integers, done! All three at zero? That's pure black, and you'll know it at a glance!

"And there's more: clamping on construction, clamping on assignment — no wild value ever reaches the screen! A pixel like this runs nineteen ninety-nine elsewhere; today it costs you nothing but the code!

"But wait — there's more! `set_red`, `set_green`, `set_blue`: three setters, on call!"

**In plain words**: Implement the RGB pixel class `Pixel`: construction and all three setters clamp every component into `0..255`; `red()`, `green()`, `blue()` read the components; `brightness()` returns the sum of the three divided by 3 using integer division; `is_black()` returns `true` only when all three components are 0.

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

### Requirements

- Every component is clamped into the closed range `0..255` during construction and assignment; inputs may lie outside it;
- `brightness()` uses integer division and stays within `0..255`;
- `is_black()` returns `true` only when all three components are 0.

Do not write `main`.
