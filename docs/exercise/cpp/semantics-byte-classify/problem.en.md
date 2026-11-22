Implement three functions that work on a single byte.

```cpp
#include <string>

enum class ByteKind { Control, Whitespace, Digit, Printable, High };

ByteKind classify_byte(unsigned char byte);
int to_lower(int ch);
std::string hex_byte(unsigned char byte);
```

`classify_byte` uses these rules (checked in order):

| Kind | Byte values |
| --- | --- |
| `Whitespace` | `9, 10, 11, 12, 13, 32` |
| `Control` | `0`–`8`, `14`–`31`, `127` |
| `Digit` | `'0'`–`'9'` |
| `High` | `128`–`255` |
| `Printable` | everything else (`33`–`126` minus the digits) |

Other requirements:

- `to_lower`: return the lowercase letter for `'A'`–`'Z'`; return every other integer (including negatives) unchanged.
- `hex_byte`: return exactly two uppercase hex digits, e.g. `0` → `"00"`, `255` → `"FF"`, `15` → `"0F"`. Note that `char` is signed on some platforms, so do not turn `0xFF` into eight hex digits.
- Do not write `main`.

### Hint

Taking the parameter as `unsigned char` guarantees a value in `0`–`255`; use `static_cast<unsigned char>(c)` before extracting the high and low nibbles. Check the narrow ranges such as whitespace and digits first.
