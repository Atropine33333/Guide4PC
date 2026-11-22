// Reference solution: types-endian-swap
#include <cstdint>
#include <iomanip>
#include <iostream>

int main() {
    std::uint32_t x = 0;
    if (!(std::cin >> x)) return 0;

    const std::uint32_t b0 = x & 0xffu;
    const std::uint32_t b1 = (x >> 8) & 0xffu;
    const std::uint32_t b2 = (x >> 16) & 0xffu;
    const std::uint32_t b3 = (x >> 24) & 0xffu;

    std::cout << b0 << ' ' << b1 << ' ' << b2 << ' ' << b3 << '\n';

    const std::uint32_t reversed = (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
    std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0') << reversed << '\n';
    return 0;
}
