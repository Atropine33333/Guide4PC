Now hear this tale of a crossing where one lamp burns in three colours. Sit still, ladies and gentlemen, and let it be told.

Since the day it was raised, the lamp has been red. Pull the lever once: red turns green. Pull again: green turns yellow. Pull again: yellow comes back to red — round and round, never out of order. Each pull writes one stroke in the ledger; `steps` pulls write `steps` strokes. The strokes in the ledger and the colour before your eyes are two separate books, and must never be confused.

There was an impatient fellow who pulled three times at once. The lamp went full circle and stood where it began, yet the ledger plainly showed three strokes.

The lamp's name must also be reported exactly: `"Red"`, `"Green"`, `"Yellow"`, not one letter off. Reporting a name needs no lamp at all — a static function will do.

**In plain words**: Implement the `TrafficLight` class: a new light is red with `ticks()` at 0; `advance()` switches once in the order `Red -> Green -> Yellow -> Red` and raises the count by one; `advance(steps)` switches `steps` times (`steps >= 0`) and raises the count by `steps`; `current()` reports the colour; `name(Light)` is static and returns the three names. `ticks()` only counts switches and has no fixed relation to the current colour.

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

### Rules

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
