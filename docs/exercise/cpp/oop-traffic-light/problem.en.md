This exercise reads no stdin and writes no stdout. Model a traffic light cycle with a class, tested by a hidden harness.

```cpp
enum class Light { Red, Yellow, Green };

class TrafficLight {
public:
    TrafficLight();
    Light current() const;
    void advance();
    void advance(int steps);
    int ticks() const;
    static const char* name(Light light);
};
```

Rules:

- A new traffic light is red and `ticks()` is 0.
- Each `advance()` switches once in the order `Red -> Green -> Yellow -> Red` and increases `ticks()` by 1.
- `advance(steps)` switches `steps` times with `steps >= 0`; `ticks()` grows by `steps`.
- `name` returns `"Red"`, `"Green"`, or `"Yellow"`.
- Do not write `main`.

### Example

| Call sequence | `current()` / `ticks()` |
| --- | --- |
| after construction | `Red` / 0 |
| `advance()` | `Green` / 1 |
| then `advance(2)` | `Red` / 3 |

### Hint

Store the state as a `Light` and switch with a `switch` statement or modular arithmetic. `ticks()` is just the number of switches and does not determine the color. `name` is static and can be called without an object.
