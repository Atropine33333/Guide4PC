#include <cstdint>

std::uint32_t rotate_left(std::uint32_t value, int count) {
    // TODO: 把移出的高位补回低位，并对 count 取模。
    return value << (count & 31);
}

std::uint32_t rotate_right(std::uint32_t value, int count) {
    // TODO: 把移出的低位补回高位，并对 count 取模。
    return value >> (count & 31);
}

std::uint32_t reverse_bits(std::uint32_t value) {
    // TODO: 逐位收集并反转。
    return value;
}
