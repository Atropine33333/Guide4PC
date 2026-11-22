// 参考实现：types-pack-bits
#include <cstdint>

namespace {

std::uint32_t clamp_channel(int value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return static_cast<std::uint32_t>(value);
}

}  // namespace

std::uint32_t pack_rgba(int r, int g, int b, int a) {
    return (clamp_channel(a) << 24) | (clamp_channel(r) << 16) |
           (clamp_channel(g) << 8) | clamp_channel(b);
}

void unpack_rgba(std::uint32_t packed, int& r, int& g, int& b, int& a) {
    a = static_cast<int>((packed >> 24) & 0xFFu);
    r = static_cast<int>((packed >> 16) & 0xFFu);
    g = static_cast<int>((packed >> 8) & 0xFFu);
    b = static_cast<int>(packed & 0xFFu);
}

std::uint32_t swap_rb(std::uint32_t packed) {
    const std::uint32_t alpha = packed & 0xFF000000u;
    const std::uint32_t red = (packed >> 16) & 0xFFu;
    const std::uint32_t green = packed & 0x00FF00u;
    const std::uint32_t blue = (packed & 0xFFu) << 16;
    return alpha | blue | green | red;
}
