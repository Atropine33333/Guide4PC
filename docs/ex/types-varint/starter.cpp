#include <cstddef>
#include <cstdint>
#include <vector>

std::vector<std::uint8_t> encode_varint(std::uint32_t value) {
    std::vector<std::uint8_t> out;
    // TODO: handle values that need more than one byte.
    out.push_back(static_cast<std::uint8_t>(value));
    return out;
}

bool decode_varint(const std::vector<std::uint8_t>& bytes,
                   std::uint32_t& value,
                   std::size_t& consumed) {
    (void)bytes;
    (void)value;
    (void)consumed;
    // TODO: decode, validate, and report the consumed length.
    return false;
}
