// 参考实现：types-bit-rotate
#include <cstdint>

namespace {

int normalize(int count) {
    int n = count % 32;
    if (n < 0) n += 32;
    return n;
}

}  // namespace

std::uint32_t rotate_left(std::uint32_t value, int count) {
    const int n = normalize(count);
    if (n == 0) return value;
    return (value << n) | (value >> (32 - n));
}

std::uint32_t rotate_right(std::uint32_t value, int count) {
    const int n = normalize(count);
    if (n == 0) return value;
    return (value >> n) | (value << (32 - n));
}

std::uint32_t reverse_bits(std::uint32_t value) {
    std::uint32_t result = 0;
    for (int i = 0; i < 32; ++i) {
        result = (result << 1) | (value & 1u);
        value >>= 1;
    }
    return result;
}
