This task reads no stdin and writes no stdout. Implement LEB128 encoding and decoding for unsigned 32-bit integers.

```cpp
#include <cstddef>
#include <cstdint>
#include <vector>

std::vector<std::uint8_t> encode_varint(std::uint32_t value);

bool decode_varint(const std::vector<std::uint8_t>& bytes,
                   std::uint32_t& value,
                   std::size_t& consumed);
```

Encoding rules:

- Split the integer into 7-bit groups, from least significant to most significant.
- Set the high bit (`0x80`) on every byte except the last one; this marks that another byte follows.
- The last byte has its high bit clear.
- `0` encodes as the single byte `0x00`.

Decoding rules:

- Read one LEB128 value starting at the beginning of `bytes`.
- On success return `true`, store the result in `value`, and store the number of bytes read in `consumed`. Extra bytes after the value are allowed.
- On failure return `false` and leave `value` and `consumed` **unchanged**.
- Fail when the input is empty, when the bytes run out before the value ends, when more than 5 bytes are needed, when the fifth byte contains bits beyond 32 bits, or when the encoding is not canonical (not the shortest form).
- Canonical form examples: `{0x80, 0x00}` and `{0x81, 0x00}` represent 0 and 1 but are not minimal, so they must be rejected.
- The fifth byte may only use its low four bits, so `{0xff, 0xff, 0xff, 0xff, 0x0f}` is valid while `{0xff, 0xff, 0xff, 0xff, 0x10}` overflows.

Do not write `main`, and do not change the signatures.
