This exercise reads no stdin and writes no stdout. Implement a variadic function template; a hidden harness runs the tests.

```cpp
template <typename... Args>
long long sum_all(Args... args);
```

Rules:

- Add all arguments as 64-bit signed integers and return the total.
- Return 0 when called with no arguments.
- Arguments may be `char`, `short`, `int`, `long`, `long long`, and so on, possibly mixed.
- A `char` contributes its numeric value (for example `'a'` is 97), not text.
- The result may be negative; do not accumulate in `int`.
- Do not write `main`.

### Hint

A C++17 fold expression is the easiest approach: `return (0LL + ... + static_cast<long long>(args));`. With an empty pack this fold produces 0.
