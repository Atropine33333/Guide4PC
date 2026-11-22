And the Lord spoke unto Moses, saying:

"Number the children of Israel in companies of seven, beginning from the least, and let them go out company by company. If a company still has companies after it, set the mark `0x80` at its head, to show that more remain; the last company shall bear no mark. And if there be not one man in the whole camp, leave a single empty company, `0x00`."

And Moses numbered them so. And when the roll was to be taken back, the Lord said further: "When you read, you shall examine: the companies shall be no more than five; the fifth company may hold only the lower four bits, and no more; and no empty or surplus company shall be left. Every form that is not the shortest shall be returned and not entered in the roll."

**In plain words**: implement LEB128 variable-length encoding and decoding for unsigned 32-bit integers. Encoding splits the value into 7-bit groups from least to most significant and sets the high bit on every byte but the last. Decoding reads one value from the start of the byte sequence, writes back the result and the number of bytes consumed, leaves the outputs untouched on failure, and rejects over-long, overflowing, or non-shortest encodings.

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
