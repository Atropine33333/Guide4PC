This exercise reads no stdin and writes no stdout. Implement three "pointer + length" functions that must return safely on a null pointer or a non-positive length, tested by a hidden harness.

```cpp
long long sum_or_zero(const int* data, int count);
int max_or(const int* data, int count, int fallback);
int count_even(const int* data, int count);
```

Rules:

- When `data == nullptr` or `count <= 0`, the three functions return `0`, `fallback`, and `0` respectively, and must not touch `data`.
- `sum_or_zero` returns the sum of all elements as a `long long`.
- `max_or` returns the largest element; an empty input returns `fallback`.
- `count_even` counts even values; negative even values count too (`-4` is even, `-3` is not).
- Do not write `main`.

### Example

| Call | Result |
| --- | --- |
| `sum_or_zero(nullptr, 5)` | `0` |
| `max_or(nullptr, 5, -7)` | `-7` |
| `sum_or_zero(values, 3)` with `values = {2, -4, 7}` | `5` |
| `count_even(values, 3)` with `values = {2, -4, 7}` | `2` |

### Hint

Write the guard as `if (data == nullptr || count <= 0) return ...;` before the loop. Even for `(nullptr, 0)`, never pass the null pointer into a loop body or into `std::max_element`.
