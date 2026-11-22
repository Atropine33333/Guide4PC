This exercise reads no stdin. Four colour channels are packed into one 32-bit unsigned integer; a hidden harness checks packing and unpacking.

```cpp
#include <cstdint>

std::uint32_t pack_rgba(int r, int g, int b, int a);
void unpack_rgba(std::uint32_t packed, int& r, int& g, int& b, int& a);
std::uint32_t swap_rb(std::uint32_t packed);
```

Layout (most significant to least significant): `a` occupies bits 24–31, `r` bits 16–23, `g` bits 8–15, `b` bits 0–7:

```
a << 24 | r << 16 | g << 8 | b
```

Rules:

- `pack_rgba` first clamps each channel to `0`–`255` (below 0 becomes 0, above 255 becomes 255), then packs them with the layout above.
- `unpack_rgba` extracts the four channels and writes them through the four reference parameters; each value lies in `0`–`255`.
- `swap_rb` exchanges the red and blue bytes and leaves green and alpha unchanged.
- Do not write `main`.

### Example

| Call | Result |
| --- | --- |
| `pack_rgba(1, 2, 3, 4)` | `0x04010203` |
| `pack_rgba(255, 0, 128, 16)` | `0x10FF0080` |
| `pack_rgba(-5, 300, 0, 999)` | `0xFF00FF00` |
| `swap_rb(0x11223344)` | `0x11443322` |

### Hint

Shift a `std::uint32_t`, not a signed `int` that might overflow; mask each extracted byte with `0xFF`.
