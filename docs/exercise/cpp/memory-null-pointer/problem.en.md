(A late-night studio. A man in a beige blazer raises three small stainless-steel gadgets.)

Tired of pointers that vanish in the night? The Pointer Trio — three functions, one price: $19.99!

`sum_or_zero`: hand it a pointer and a length, and it hands back the sum. Give it `nullptr` or `count <= 0` and it answers `0` without touching a single byte of memory.

But wait, there's more! `max_or` finds the largest value, and on empty input it returns the `fallback` you supplied. No crash, no phone call at three in the morning.

And `count_even`? It counts the even ones. Negative evens count too: `-4` is in, `-3` is out.

Order now and we'll throw in a guard clause, free: `if (data == nullptr || count <= 0) return ...;` — written before the loop, at no extra charge.

Operators are standing by. Do not write `main`. Void where prohibited.

**In plain words**: implement three pointer-plus-length functions. When `data == nullptr` or `count <= 0` they return `0`, `fallback`, and `0`, and must not touch `data`. `sum_or_zero` sums into a `long long`; `max_or` returns the largest element; `count_even` counts even values, negatives included.

```cpp
long long sum_or_zero(const int* data, int count);
int max_or(const int* data, int count, int fallback);
int count_even(const int* data, int count);
```

### Example

| Call | Result |
| --- | --- |
| `sum_or_zero(nullptr, 5)` | `0` |
| `max_or(nullptr, 5, -7)` | `-7` |
| `sum_or_zero(values, 3)` with `values = {2, -4, 7}` | `5` |
| `count_even(values, 3)` with `values = {2, -4, 7}` | `2` |

### Hint

Write the guard as `if (data == nullptr || count <= 0) return ...;` before the loop. Even for `(nullptr, 0)`, never pass the null pointer into a loop body or into `std::max_element`.
