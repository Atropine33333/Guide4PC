#include <cstdint>

std::uint32_t pack_rgba(int r, int g, int b, int a) {
    (void)r;
    (void)g;
    (void)b;
    (void)a;
    // TODO: 钳制到 0..255，再按 a/r/g/b 的顺序移位拼接。
    return 0;
}

void unpack_rgba(std::uint32_t packed, int& r, int& g, int& b, int& a) {
    (void)packed;
    // TODO: 分别取出四个字节。
    r = 0;
    g = 0;
    b = 0;
    a = 0;
}

std::uint32_t swap_rb(std::uint32_t packed) {
    // TODO: 只交换红色和蓝色字节。
    return packed;
}
