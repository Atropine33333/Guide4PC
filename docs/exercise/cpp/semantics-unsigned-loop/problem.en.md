This exercise reads no stdin. Implement three functions; a hidden harness checks how well you understand signed/unsigned conversion and wrap-around.

```cpp
bool unsigned_less(int a, unsigned b);
unsigned wrap_decrement(unsigned value, unsigned steps);
int countdown_loops(unsigned start);
```

Rules:

- `unsigned_less(a, b)` returns the mathematical `a < b`. Since `a` may be negative and `b` may exceed `INT_MAX`, plain `a < b` is wrong: it converts `a` to `unsigned`, making negative values huge.
- `wrap_decrement(value, steps)` returns `value - steps` modulo 2³², always landing in `[0, 2³²)`; underflow continues from `UINT_MAX`.
- `countdown_loops(start)` counts how many times the body of `for (unsigned i = start; i > 0; --i) { ... }` runs. `start` is at most `INT_MAX`, so the count fits in `int`.
- Do not write `main`.

### Example

| Call | Result |
| --- | --- |
| `unsigned_less(-1, 0)` | `true` |
| `unsigned_less(INT_MAX, 2147483648u)` | `true` |
| `wrap_decrement(0, 1)` | `4294967295` |
| `wrap_decrement(5, 3)` | `2` |
| `countdown_loops(5)` | `5` |
| `countdown_loops(0)` | `0` |

### Hint

When comparing a signed value with an unsigned one, handle `a < 0` first and then compare both as unsigned. Unsigned subtraction already wraps modulo 2³², so no manual branch is needed.
