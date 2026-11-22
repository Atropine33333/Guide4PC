This exercise reads no stdin. Implement a simple TLV (tag-length-value) record codec.

```cpp
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

struct DecodeResult {
    bool ok;
    std::vector<std::pair<unsigned char, std::string>> records;
    std::size_t bad_offset;
};

std::vector<unsigned char> encode_records(
    const std::vector<std::pair<unsigned char, std::string>>& records);

DecodeResult decode_records(const std::vector<unsigned char>& data);
```

The encoding of each record is:

1. one tag byte;
2. one length byte `len` (the payload size, 0 to 255);
3. exactly `len` payload bytes.

There is no separator between records and no overall header.

Rules:

- `encode_records` concatenates records in input order; payload lengths never exceed 255.
- `decode_records` parses from left to right. On complete success `ok = true` and `bad_offset = 0`.
- If the remaining bytes cannot form a complete record (missing tag, missing length, or truncated payload), `ok = false`, `records` contains only the records decoded before the failure, and `bad_offset` is the index of the **first byte of the failing record** in the original array.
- Empty input is valid and returns `ok = true`, an empty `records`, and `bad_offset = 0`.
- Do not write `main`.

### Example

Records `{(1, "A"), (2, "BC")}` encode to `01 01 41 02 02 42 43`.

Decoding `{01 01 41 02 05 42}`: the first record is fine; the second claims length `5` but only one byte remains, so `ok = false`, `records = {(1, "A")}`, and `bad_offset = 3`.

### Hint

Walk with a `std::size_t offset`; first check `data.size() - offset >= 2`, then check that the payload is complete. A `std::string` can be built as `std::string(begin, length)`.
