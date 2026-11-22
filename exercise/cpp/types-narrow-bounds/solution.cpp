// 参考实现：types-narrow-bounds
#include <cstdint>
#include <limits>

bool narrow_i8(long long value, std::int8_t& out) {
    if (value < std::numeric_limits<std::int8_t>::min() ||
        value > std::numeric_limits<std::int8_t>::max()) {
        return false;
    }
    out = static_cast<std::int8_t>(value);
    return true;
}

bool narrow_i16(long long value, std::int16_t& out) {
    if (value < std::numeric_limits<std::int16_t>::min() ||
        value > std::numeric_limits<std::int16_t>::max()) {
        return false;
    }
    out = static_cast<std::int16_t>(value);
    return true;
}

bool narrow_u8(long long value, std::uint8_t& out) {
    if (value < 0 || value > std::numeric_limits<std::uint8_t>::max()) {
        return false;
    }
    out = static_cast<std::uint8_t>(value);
    return true;
}

bool narrow_i32(long long value, std::int32_t& out) {
    if (value < std::numeric_limits<std::int32_t>::min() ||
        value > std::numeric_limits<std::int32_t>::max()) {
        return false;
    }
    out = static_cast<std::int32_t>(value);
    return true;
}
