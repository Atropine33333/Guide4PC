Ordinance on Exact Float-to-Integer Conversion

Article 1. This Ordinance governs exact conversion from `double` to `int`, with the function form set out below:

```cpp
bool to_int_exact(double value, int& out);
```

Article 2. Conversion is granted only when all three conditions hold: `value` is finite; `value` equals some integer; that integer lies between `-2147483648` and `2147483647`. When they hold, write the result to `out` and return `true`.
Article 3. In every other case return `false` and leave `out` at its original value.
Article 4. `NaN`, positive infinity, and negative infinity are all refused. `-0.0` is admitted, and yields `0`.
Article 5. A conversion performed before the check has already produced undefined behaviour at the moment it went out of range; this Ordinance recognizes no effect in it. `2147483648.0` must be rejected.
Article 6. No `main` shall be written.

Appendix. Order of review: first `std::isfinite(value)`, then `value == std::trunc(value)`, and last a comparison against the `int` bounds. Cast first and compare later, and you fall under Article 5.

**In plain words**: implement one exact conversion function. When the `double` is finite, is exactly an integer, and that integer fits in `int`, write it to `out` and return `true`; otherwise return `false` and leave `out` alone.
