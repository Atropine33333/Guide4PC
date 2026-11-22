Welcome to the Alignment Camp.

Every rule here is true. You took no wrong turn; common sense simply does not land inside the gate.

Rule One: The camp's boundaries are 1, 2, 4, 8 and their kind. When `alignment` is a power of two, everything proceeds by boundary.

Rule Two: `align_up(value, alignment)` walks a man to the first boundary not below `value`. `padding_for(used, alignment)` reports how many steps remain from `used` to that boundary, and the number is exactly `align_up(used, alignment) - used`.

Rule Three: `is_aligned(pointer, alignment)` reads only whether the house number is a multiple of `alignment`. Convert the address to `std::uintptr_t` first; the eye cannot see it from the outside.

Rule Four: With a boundary of zero, the camp keeps no boundary at all. `align_up` lets men through unchanged and `padding_for` reports 0; `is_aligned` answers `false` without exception.

Rule Five: The empty address stands at house number zero. When `alignment` is a positive power of two, `is_aligned(nullptr, alignment)` answers `true`.

Rule Six: When `alignment` is no power of two, `is_aligned` answers `false`; `align_up` and `padding_for` may compute as they please, provided nothing explodes.

Rule Seven: A man who steps past the end of `std::size_t` is not wrapped around. The camp issues a single return ticket marked `SIZE_MAX`.

Rule Eight: The camp has no front door. Do not write `main`.

**In plain words**: implement three functions. `align_up` returns the smallest aligned value not less than `value`; `padding_for` returns the bytes needed to pad up to the boundary; `is_aligned` reports whether a pointer address is a multiple of `alignment`. With `alignment == 0` they return `value`, `0`, and `false`. `is_aligned(nullptr, positive power of two)` returns `true`. With a non-power-of-two `alignment`, `is_aligned` returns `false` and the other two only need to avoid crashing. A result beyond the range of `std::size_t` returns `SIZE_MAX`. This exercise reads no stdin and writes no stdout.

```cpp
#include <cstddef>

std::size_t align_up(std::size_t value, std::size_t alignment);
bool is_aligned(const void* pointer, std::size_t alignment);
std::size_t padding_for(std::size_t used, std::size_t alignment);
```

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
