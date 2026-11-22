This exercise reads no stdin. Implement rotations and bit reversal for 32-bit unsigned integers; a hidden harness checks them.

```cpp
#include <cstdint>

std::uint32_t rotate_left(std::uint32_t value, int count);
std::uint32_t rotate_right(std::uint32_t value, int count);
std::uint32_t reverse_bits(std::uint32_t value);
```

Rules:

- `rotate_left` moves bits out of the top back into the bottom; `rotate_right` goes the other way.
- `count` may be any integer: reduce it modulo 32 first, and a negative count rotates in the opposite direction. Rotating left by 33 is the same as by 1; rotating left by `-1` is rotating right by 1.
- When the reduced count is 0, return the value unchanged; never shift a 32-bit value by 32 or by a negative amount.
- `reverse_bits` swaps bit 0 with bit 31, bit 1 with bit 30, and so on.
- Do not write `main`.

### Example

| Call | Result |
| --- | --- |
| `rotate_left(0x80000001u, 1)` | `0x00000003u` |
| `rotate_left(0x12345678u, 8)` | `0x34567812u` |
| `rotate_right(0x12345678u, 8)` | `0x78123456u` |
| `rotate_right(1u, 1)` | `0x80000000u` |
| `reverse_bits(0x12345678u)` | `0x1E6A2C48u` |
| `reverse_bits(0x0000FFFFu)` | `0xFFFF0000u` |

### Hint

Normalise the count into `[0, 32)` first: `int n = count % 32; if (n < 0) n += 32;`. Return the value unchanged when `n == 0` so you never write `value >> 32`.
