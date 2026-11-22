This exercise reads no stdin and writes no stdout. Implement a function that decides whether two `double` values are close enough, tested by a hidden harness.

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
