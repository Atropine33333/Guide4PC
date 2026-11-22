This exercise reads no stdin and writes no stdout. Implement three checked integer arithmetic functions; a hidden harness runs the tests.

```cpp
bool add_ints(int a, int b, int& out);
bool sub_ints(int a, int b, int& out);
bool mul_ints(int a, int b, int& out);
```

Rules:

- When the result fits in `int`, store it in `out` and return `true`.
- When the result is outside the `int` range, return `false` and leave `out` **unchanged**.
- Never compute `a + b`, `a - b`, or `a * b` directly as `int` when it might overflow: that is undefined behaviour.
- Do not write `main`.

### Hint

Convert the operands to `long long` before computing. The sum, difference, and product of two `int` values always fit in `long long`, so comparing the result against the `int` bounds is safe.
