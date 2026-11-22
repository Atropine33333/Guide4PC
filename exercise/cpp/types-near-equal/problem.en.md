Huizi said to Zhuangzi: "A rod one foot long, halved each day, will never be exhausted in ten thousand ages. So then — two numbers drawn close together: how close must they be before we call them equal?"

Zhuangzi said: "Measure them by tolerance. `tolerance` is the difference you are willing to allow. A difference within it is called equal; a difference beyond it is called unequal. And tolerance may not be negative, for a negative allowance is nonsense."

Huizi said: "And if the two are already equal, where is the difference to come from?"

Zhuangzi said: "Test equality first, and the finite cases and the same-signed infinities are settled at once, with no subtraction performed."

Huizi said: "And if one is infinite and the other finite?"

Zhuangzi said: "Between the infinite and the finite the difference has no meaning; nor between positive and negative infinity. Both kinds are called unequal."

Huizi said: "And if both are `NaN`?"

Zhuangzi said: "`NaN` is unequal to all things, and unequal even to itself. This rule cannot be broken."

**In plain words**: implement the function below. `tolerance ≥ 0`; test `a == b` first, which also covers same-signed infinities; otherwise, when both are finite, test `|a - b| ≤ tolerance`; if either side is `NaN` return `false`, and that includes two `NaN`s.

```cpp
bool nearly_equal(double a, double b, double tolerance);
```

Rules:

- `tolerance ≥ 0`.
- Return `true` when the two values are equal; otherwise return `true` when `|a - b| ≤ tolerance`.
- Infinities with the same sign count as equal and return `true`; infinity against a finite value, or positive infinity against negative infinity, returns `false`.
- If either value is NaN, return `false` (even when both are NaN).
- Do not write `main`.

### Hint

Check `a == b` first; it covers both "equal finite values" and "equal infinities of the same sign". Then check `std::isfinite`. If you just write `std::fabs(a - b) <= tolerance`, `inf - inf` becomes NaN and the result turns into `false`.
