This exercise reads no stdin and writes no stdout. Implement an exact `double`-to-`int` conversion; a hidden harness runs the tests.

```cpp
bool to_int_exact(double value, int& out);
```

Rules:

- When `value` is finite, equals an integer, and that integer fits in `int` (`-2147483648` to `2147483647`), store the result in `out` and return `true`.
- In every other case return `false` and leave `out` **unchanged**.
- `NaN`, positive infinity, and negative infinity all return `false`; `-0.0` converts to `0`.
- Do not write `main`.

### Hint

First check `std::isfinite(value)`, then `value == std::trunc(value)`, and finally compare against the `int` bounds. Do not cast first: compare before converting so out-of-range values never cause undefined behaviour. Note that `2147483648.0` must be rejected.
