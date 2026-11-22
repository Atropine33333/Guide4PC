Implement four functions for a dynamic array.

```cpp
int* make_zero_array(int count);
void fill_sequence(int* data, int count, int start);
long long sum_array(const int* data, int count);
void free_array(int* data);
```

Rules:

- `make_zero_array(count)`: return `nullptr` when `count <= 0`; otherwise allocate with `new int[count]()` so every element is 0.
- `fill_sequence(data, count, start)`: set `data[i]` to `start + i`; do nothing when `count <= 0`.
- `sum_array(data, count)`: return the sum of the first `count` elements; return 0 when `count <= 0` or `data == nullptr`.
- `free_array(data)`: free an array returned by `make_zero_array`; it must accept `nullptr`.
- Do not write `main`.

### Hint

The empty parentheses in `new int[n]()` value-initialize the elements to 0. Match allocation and deallocation; never mix `new[]` with `delete`.
