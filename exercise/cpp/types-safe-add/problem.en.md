The Code of Signed Integer Arithmetic

Article 1. This Code governs the three operations of addition, subtraction, and multiplication on `int`. Each has its own function, in the form set out below:

```cpp
bool add_ints(int a, int b, int& out);
bool sub_ints(int a, int b, int& out);
bool mul_ints(int a, int b, int& out);
```

Article 2. Where the result falls within the range of `int`, write it to `out` and return `true`.
Article 3. Where the result falls outside the range of `int`, return `false` and leave `out` at its original value, unaltered in the slightest.
Article 4. Computing `a + b`, `a - b`, or `a * b` directly as `int` when it may overflow is undefined behaviour, and this Code recognizes no effect in it.
Article 5. The review shall be settled before the operation, never after it. Whoever computes first and checks second has overflow for a fact, and no remedy arrives in time.
Article 6. No `main` shall be written.

Appendix. The operands may be converted to `long long` before computing. The sum, difference, and product of two `int` values always fall within `long long`, so they may safely be compared against the `int` bounds — and only after that comparison do you decide whether `out` is written.

**In plain words**: implement three checked arithmetic functions. When the result fits in `int`, write it to `out` and return `true`; on overflow return `false` and leave `out` alone.
