This exercise reads no stdin and writes no stdout. Implement three functions that align pointers and sizes, tested by a hidden harness.

```cpp
#include <cstddef>

std::size_t align_up(std::size_t value, std::size_t alignment);
bool is_aligned(const void* pointer, std::size_t alignment);
std::size_t padding_for(std::size_t used, std::size_t alignment);
```

Rules:

- When `alignment` is a power of two (1, 2, 4, 8, ...) the usual alignment rules apply.
- `align_up(value, alignment)` returns the smallest aligned value not less than `value`.
- `padding_for(used, alignment)` returns the number of bytes needed to pad `used` up to an aligned value, i.e. `align_up(used, alignment) - used`.
- `is_aligned(pointer, alignment)` reports whether the pointer address is a multiple of `alignment`.
- Special cases:
  - When `alignment == 0`, `align_up` returns `value` unchanged, `padding_for` returns 0, and `is_aligned` returns `false`.
  - `is_aligned(nullptr, alignment)` returns `true` when `alignment` is positive and a power of two.
  - When `alignment` is not a power of two, `is_aligned` returns `false`; `align_up` and `padding_for` only need to avoid crashing (the bit-mask formula is fine).
  - When the aligned result exceeds the range of `std::size_t`, return `SIZE_MAX` instead of wrapping around.
- Do not write `main`.

### Example

| Call | Result |
| --- | --- |
| `align_up(5, 8)` | `8` |
| `align_up(8, 8)` | `8` |
| `padding_for(9, 8)` | `7` |
| `is_aligned(nullptr, 8)` | `true` |
| `is_aligned(buffer + 1, 16)` | `false` (when `buffer` is 16-aligned) |

### Hint

Test a power of two with `alignment != 0 && (alignment & (alignment - 1)) == 0`. Convert a pointer address to `std::uintptr_t` before taking a modulus. In `align_up`, check `value > SIZE_MAX - (alignment - 1)` first, then compute `(value + alignment - 1) & ~(alignment - 1)`.
