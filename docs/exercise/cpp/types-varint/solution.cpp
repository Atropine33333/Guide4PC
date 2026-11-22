// Reference solution: types-varint
#include <cstddef>
#include <cstdint>
#include <vector>

std::vector<std::uint8_t> encode_varint(std::uint32_t value) {
    std::vector<std::uint8_t> out;
    while (value >= 0x80u) {
        out.push_back(static_cast<std::uint8_t>((value & 0x7fu) | 0x80u));
        value >>= 7;
    }
    out.push_back(static_cast<std::uint8_t>(value));
    return out;
}

bool decode_varint(const std::vector<std::uint8_t>& bytes,
                   std::uint32_t& value,
                   std::size_t& consumed) {
    std::uint64_t result = 0;
    for (std::size_t i = 0; i < 5; ++i) {
        if (i >= bytes.size()) return false;
        const std::uint8_t byte = bytes[i];
        result |= static_cast<std::uint64_t>(byte & 0x7fu) << (7 * i);
        if ((byte & 0x80u) != 0) {
            if (i == 4) return false;
            continue;
        }
        if (i == 4 && (byte & 0xf0u) != 0) return false;
        if (i > 0 && byte == 0) return false;
        if (result > 0xffffffffull) return false;
        value = static_cast<std::uint32_t>(result);
        consumed = i + 1;
        return true;
    }
    return false;
}
