// 参考实现：types-float-decode
#include <cstdint>
#include <iostream>
#include <string>

int main() {
    std::string text;
    if (!(std::cin >> text)) return 0;

    std::uint32_t bits = 0;
    for (char c : text) {
        int digit = -1;
        if (c >= '0' && c <= '9') digit = c - '0';
        else if (c >= 'a' && c <= 'f') digit = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F') digit = c - 'A' + 10;
        if (digit < 0) return 0;
        bits = (bits << 4) | static_cast<std::uint32_t>(digit);
    }

    const unsigned sign = (bits >> 31) & 1u;
    const unsigned exponent = (bits >> 23) & 0xFFu;
    const unsigned mantissa = bits & 0x7FFFFFu;

    const char *kind = "normal";
    if (exponent == 255) {
        kind = mantissa == 0 ? "inf" : "nan";
    } else if (exponent == 0) {
        kind = mantissa == 0 ? "zero" : "subnormal";
    }

    std::cout << kind << ' ' << sign << ' ' << exponent << ' ' << mantissa << '\n';
    return 0;
}
